// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/ml/webnn/graph_nnapi_impl.h"

#include "base/memory/ptr_util.h"
#include "content/browser/ml/webnn/nnapi/GraphNnapi.h"
#include "mojo/public/cpp/bindings/self_owned_receiver.h"

namespace content::webnn {

namespace {

using ml::webnn::mojom::BuildResult;
using ml::webnn::mojom::ComputeResult;
using ml::webnn::mojom::Graph;

}  // namespace

// static
void GraphNnapiImpl::Create(mojo::PendingReceiver<Graph> receiver) {
  mojo::MakeSelfOwnedReceiver<Graph>(base::WrapUnique(new GraphNnapiImpl()),
                                     std::move(receiver));
}

GraphNnapiImpl::~GraphNnapiImpl() = default;

GraphNnapiImpl::GraphNnapiImpl()
    : native_graph_nnapi_(std::make_unique<GraphNnapiNativeImpl>()) {}

void GraphNnapiImpl::AddInput(const std::string& name,
                            OperandDescriptorPtr desc) {
  // TODO: return directly if BuildResult has error message.
  native_graph_nnapi_->AddInput(name, std::move(desc));
}

void GraphNnapiImpl::AddConstant(OperandDescriptorPtr desc,
                               const std::vector<uint8_t>& array_buffer) {
  // TODO: return directly if BuildResult has error message.
  native_graph_nnapi_->AddConstant(std::move(desc), array_buffer);
}

void GraphNnapiImpl::AddElementWiseBinary(uint32_t a_id,
                                        uint32_t b_id,
                                        BinaryOperandType type,
                                        OperandDescriptorPtr output_desc) {
  // TODO: return directly if BuildResult has error message.
  native_graph_nnapi_->AddElementWiseBinary(a_id, b_id, type,
                                          std::move(output_desc));

void GraphNnapiImpl::Build(
    const base::flat_map<std::string, uint32_t>& named_operands,
    BuildCallback callback) {
  std::move(callback).Run(native_graph_nnapi_->CompileImpl(named_operands));
}

void GraphNnapiImpl::Compute(NamedInputsPtr named_inputs,
                           ComputeCallback callback) {
  auto named_outputs = ml::webnn::mojom::NamedOutputs::New();
  ComputeResult result =
      native_graph_nnapi_->ComputeImpl(std::move(named_inputs), named_outputs);
  std::move(callback).Run(result, std::move(named_outputs));
}                                         