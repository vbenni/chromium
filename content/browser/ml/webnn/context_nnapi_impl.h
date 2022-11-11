// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ML_WEBNN_CONTEXT_NNAPI_IMPL_H_
#define CONTENT_BROWSER_ML_WEBNN_CONTEXT_NNAPI_IMPL_H_

#include "components/ml/mojom/webnn_context.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/system/handle.h"

namespace content::webnn {

class ContextNnapiImpl : public ml::webnn::mojom::Context {
 public:
  ~ContextNnapiImpl() override;
  static void Create(mojo::PendingReceiver<ml::webnn::mojom::Context> receiver);

  ContextNnapiImpl(const ContextNnapiImpl&) = delete;
  ContextNnapiImpl& operator=(const ContextNnapiImpl&) = delete;

 protected:
  ContextNnapiImpl();

 private:
  // ml::webnn::mojom::context
  void CreateGraph(uint32_t, CreateGraphCallback) override;
};

}  // namespace content::webnn

#endif  // CONTENT_BROWSER_ML_WEBNN_CONTEXT_NNAPI_IMPL_H_
