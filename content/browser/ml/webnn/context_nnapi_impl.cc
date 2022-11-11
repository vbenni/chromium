// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/ml/webnn/context_nnapi_impl.h"

#include "base/memory/ptr_util.h"
#include "content/browser/ml/webnn/graph_nnapi_impl.h"
#include "mojo/public/cpp/bindings/self_owned_receiver.h"

namespace content::webnn {

namespace {

using ml::webnn::mojom::Graph;

}  // namespace

// static
void ContextNnapiImpl::Create(mojo::PendingReceiver<Context> receiver) {
  mojo::MakeSelfOwnedReceiver<Context>(base::WrapUnique(new ContextNnapiImpl()),
                                       std::move(receiver));
}

ContextNnapiImpl::~ContextNnapiImpl() = default;

ContextNnapiImpl::ContextNnapiImpl() = default;

void ContextNnapiImpl::CreateGraph(uint32_t graph_id,
                                 CreateGraphCallback callback) {
  // TODO: Save the graph id in the native graph.
  // The remote sent to the renderer.
  mojo::PendingRemote<Graph> blink_remote;
  // The receiver bind to GraphNnapiImpl.
  GraphNnapiImpl::Create(blink_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(std::move(blink_remote));
}

}  // namespace content::webnn
