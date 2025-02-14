/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "fbpcf/io/cloud_util/GCSFileReader.h"
#include <fbpcf/gcp/GCSUtil.h>
#include <google/cloud/storage/download_options.h>
#include "fbpcf/exception/GcpException.h"

namespace fbpcf::cloudio {

template <class ClientCls>
std::string GCSFileReader<ClientCls>::readBytes(
    const std::string& filePath,
    std::size_t start,
    std::size_t end) {
  const auto& ref = fbpcf::gcp::uriToObjectReference(filePath);

  auto outcome = GCSClient_->ReadObject(
      ref.bucket, ref.key, google::cloud::storage::ReadRange(start, end));
  if (!outcome.status().ok()) {
    throw GcpException{outcome.status().message()};
  }
  std::stringstream ss;
  ss << outcome.rdbuf();
  return ss.str();
}

template <class ClientCls>
size_t GCSFileReader<ClientCls>::getFileContentLength(
    const std::string& filePath) {
  const auto& ref = fbpcf::gcp::uriToObjectReference(filePath);
  auto outcome = GCSClient_->GetObjectMetadata(ref.bucket, ref.key);
  if (!outcome) {
    throw GcpException{"Error getting object metadata for object " + ref.key};
  }
  return outcome.size();
}

} // namespace fbpcf::cloudio
