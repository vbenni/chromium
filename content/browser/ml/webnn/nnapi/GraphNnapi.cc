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

#include "content/browser/ml/webnn/nnapi/GraphNnapi.h"

#include <algorithm>

#include "nnapiUtils.h"
#include "base/logging.h"
#include "content/browser/ml/webnn/fusion_operators.h"
#include "content/browser/ml/webnn/graph_nnapi_impl.h"

namespace content::webnn {

namespace {

using ml::webnn::mojom::AutoPad;
using ml::webnn::mojom::Conv2dFilterOperandLayout;
using ml::webnn::mojom::Conv2dOptions;
using ml::webnn::mojom::Conv2dOptionsPtr;
using ml::webnn::mojom::FusionOperator;
using ml::webnn::mojom::FusionOperatorPtr;
using ml::webnn::mojom::FusionType;
using ml::webnn::mojom::InputOperandLayout;
using ml::webnn::mojom::MemoryInfoPtr;
using ml::webnn::mojom::OperandType;

}  // namespace

NnapiTensorDesc::NnapiTensorDesc() = default;
NnapiTensorDesc::~NnapiTensorDesc() = default;

MemoryInfo::MemoryInfo() = default;
MemoryInfo::~MemoryInfo() = default;

#define DAWN_INTERNAL_ERROR(MESSAGE)            \
  do {                                          \
    error_messages_ = MESSAGE;                  \
    assert(0);                                  \
    build_result_ = BuildResult::kUnknownError; \
    return;                                     \
  } while (0)



void GraphNnapiNativeImpl::AddInput(const std::string& name,
                                  OperandDescriptorPtr desc) {
  //const OperandDescriptor* desc = input->GetOperandDescriptor();
  std::shared_ptr<NnapiTensorDesc> nnapiTensorDesc(new NnapiTensorDesc);
  if (!CreateNnapiTensorDesc(mNnapiTensorsDesc, nnapiTensorDesc, desc)) {
    DAWN_INTERNAL_ERROR("Failed to create NNAPI tensor description.");
  }
  NNAPI_TENSOR_DESC outputTensorDESC = {NNAPI_TENSOR_TYPE_BUFFER,
                                      &(nnapiTensorDesc->bufferDesc)};

  //TODO:


  auto desc = input->GetOperandDescriptor();
  auto node = CreateIOOperand(name, desc, true);
  DAWN_TRY(CheckForNullNode(node, "Failed to create Input operand"));
  mGraphNodeMap[input->PrimaryOutput()] = node->opIndex;

  return;
}