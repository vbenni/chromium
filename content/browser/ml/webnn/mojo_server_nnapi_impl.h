// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ML_WEBNN_MOJO_SERVER_NNAPI_IMPL_H_
#define CONTENT_BROWSER_ML_WEBNN_MOJO_SERVER_NNAPI_IMPL_H_

#include "components/ml/mojom/webnn_service.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

namespace content::webnn {

class MojoServerNnapiImpl : public ml::webnn::mojom::MojoServer {
 public:
  ~MojoServerNnapiImpl() override;
  static void Create(
      mojo::PendingReceiver<ml::webnn::mojom::MojoServer> receiver);

  MojoServerNnapiImpl(const MojoServerNnapiImpl&) = delete;
  MojoServerNnapiImpl& operator=(const MojoServerNnapiImpl&) = delete;

 protected:
  MojoServerNnapiImpl();

 private:
  // ml::webnn::mojom::MojoServer
  void CreateContext(ml::webnn::mojom::ContextOptionsPtr options,
                     uint32_t context_id,
                     CreateContextCallback callback) override;
};

}  // namespace content::webnn

#endif  // CONTENT_BROWSER_ML_WEBNN_MOJO_SERVER_NNAPI_IMPL_H_
