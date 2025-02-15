//===- DXILMetadata.h - DXIL Metadata helper objects ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains helper objects for working with DXIL metadata.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_DIRECTX_DXILMETADATA_H
#define LLVM_TARGET_DIRECTX_DXILMETADATA_H

namespace llvm {
class Module;
class NamedMDNode;
class VersionTuple;
namespace dxil {

class ValidatorVersionMD {
  NamedMDNode *Entry;

public:
  ValidatorVersionMD(Module &M);

  void update(VersionTuple ValidatorVer);

  bool isEmpty();
};

} // namespace dxil
} // namespace llvm

#endif // LLVM_TARGET_DIRECTX_DXILMETADATA_H
