// Copyright 2021 The WebNN-native Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CONTENT_BROWSER_ML_WEBNN_NNAPI_GRAPHNNAPI_H_
#define CONTENT_BROWSER_ML_WEBNN_NNAPI_GRAPHNNAPI_H_

#define DML_TARGET_VERSION_USE_LATEST 1

#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <wrl\client.h>
#include <unordered_map>
#include <unordered_set>

#include "base/memory/read_only_shared_memory_region.h"
#include "base/memory/shared_memory_mapping.h"
#include "components/ml/mojom/webnn_graph.mojom.h"

namespace content::webnn {

using ml::webnn::mojom::BinaryOperandType;
using ml::webnn::mojom::BuildResult;
using ml::webnn::mojom::ClampOptions;
using ml::webnn::mojom::ClampOptionsPtr;
using ml::webnn::mojom::ComputeResult;
using ml::webnn::mojom::Conv2dOptionsPtr;
using ml::webnn::mojom::FusionOperator;
using ml::webnn::mojom::GemmOptionsPtr;
using ml::webnn::mojom::NamedInputsPtr;
using ml::webnn::mojom::NamedOutputsPtr;
using ml::webnn::mojom::OperandDescriptorPtr;
using ml::webnn::mojom::Pool2dOptions;
using ml::webnn::mojom::Pool2dOptionsPtr;
using ml::webnn::mojom::Pool2dType;
using ml::webnn::mojom::UnaryOperandType;

class FusionOperators;

struct MemoryInfo {
  MemoryInfo();
  ~MemoryInfo();
  size_t byte_offset;
  size_t byte_length;
};

// Represent the NNAPI tensor description.
struct NnapiTensorDesc {
  NnapiTensorDesc();
  ~NnapiTensorDesc();
  std::vector<UINT> dimensions = {};
  std::vector<UINT> strides = {};
  // Describes a tensor that will be stored in a Direct3D 12 buffer resource.
  NNAPI_BUFFER_TENSOR_DESC bufferDesc = {};
};

// Represent the information of the graph's edges.
struct EdgeInfoBase {
  virtual ~EdgeInfoBase() = default;
  NNAPI_TENSOR_DESC outputTensorDESC = {};
  std::string name = "";
  bool isInputEdge = false;
};

// Only represent the information of the input edges.
struct InputEdgeInfo final : public EdgeInfoBase {
  ~InputEdgeInfo() override = default;
  // Indicate the index of the graph's input.
  size_t inputIndex = 0;
  void const* buffer = nullptr;
  size_t byteLength = 0;
  // Indicate if the input is from constant buffer which need to be
  // uploaded in the stage of initialization.
  bool isConstantInput = false;
};

// Represent the information of the intermediate edges and output edges.
struct EdgeInfo final : public EdgeInfoBase {
  ~EdgeInfo() override = default;
  // Indicate the index of the intermediate node from which this edge was
  // produced.
  uint32_t nodeIndex = 0;
  // Indicate the index of the intermediate node' output from which this edge
  // was produced.
  uint32_t outputNodeIndex = 0;
};

class GraphNnapiNativeImpl {
 public:
  GraphNnapiNativeImpl();
  ~GraphNnapiNativeImpl();

  void AddConstant(OperandDescriptorPtr, const std::vector<uint8_t>&);
  void AddInput(const std::string&, OperandDescriptorPtr);
  void AddOutput(const std::string&, uint32_t);
  void AddElementWiseBinary(uint32_t,
                            uint32_t,
                            BinaryOperandType,
                            OperandDescriptorPtr);
  // void AddConv2d(uint32_t, uint32_t, Conv2dOptionsPtr, OperandDescriptorPtr);
  // void AddPool2d(uint32_t input_id,
  //                Pool2dOptionsPtr options,
  //                Pool2dType type,
  //                OperandDescriptorPtr desc);
  // void AddReshape(uint32_t, OperandDescriptorPtr);
  // void AddUnary(uint32_t, UnaryOperandType, OperandDescriptorPtr);
  // void AddGemm(uint32_t a_id,
  //              uint32_t b_id,
  //              GemmOptionsPtr options,
  //              OperandDescriptorPtr desc);
  // void AddClamp(uint32_t, ClampOptionsPtr, OperandDescriptorPtr);
  // void AddFusionClamp(ClampOptionsPtr options, uint32_t operator_id);
  BuildResult CompileImpl(
      const base::flat_map<std::string, uint32_t>& named_operands);
  ComputeResult ComputeImpl(NamedInputsPtr named_inputs,
                            NamedOutputsPtr& named_outputs);

  // void AddEdgesToThisNode(
  //     std::vector<std::shared_ptr<EdgeInfoBase>> inputNodes);
  void FillUploadResourceAndInputBindings(
      uint64_t uploadResourceSize,
      std::vector<NNAPI_BUFFER_BINDING>& inputBufferBinding,
      NamedInputsPtr namedInputs);
  std::shared_ptr<EdgeInfoBase> Clamp(std::shared_ptr<EdgeInfoBase> inputEdge,
                                      const ClampOptions* options);
  void EmulateFusedOperator(const FusionOperator* activation,
                            std::shared_ptr<EdgeInfoBase>& inputEdge,
                            const std::vector<UINT>& inputDims);
  void TransposeOutputToNhwc(std::shared_ptr<EdgeInfoBase>& inputEdge,
                             const std::vector<UINT>& nchwOutputDims);

 private:
  BuildResult Finish();

};

}  // namespace content::webnn

#endif  // CONTENT_BROWSER_ML_WEBNN_NNAPI_GRAPHNNAPI_H_