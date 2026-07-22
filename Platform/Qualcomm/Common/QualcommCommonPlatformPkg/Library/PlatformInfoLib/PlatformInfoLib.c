/** @file

  Interface implementation file for the PlatformInfo driver.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/SmemLib.h>
#include "PlatformInfoInternal.h"

/**
  Initializes the PlatformInfo driver.

  @retval PLATFORMINFO_SUCCESS  Initialization completed successfully.
  @retval PLATFORMINFO_ERROR    Failed to retrieve platform information from SMEM.
**/
PlatformInfoResult
EFIAPI
PlatformInfoInit (
  VOID
  )
{
  PlatformInfoDrvCtxt   *DrvCtxt;
  UINT32                Size;
  PlatformInfoSmemType  *Smem;
  PlatformInfoKvpsType  *Kvps;

  DrvCtxt = PlatformInfoGetDrvCtxt ();

  if (DrvCtxt->Initialized) {
    return PLATFORMINFO_SUCCESS;
  }

  Smem = SmemGetAddr (SmemHwSwBuildId, &Size);
  if (Smem == NULL) {
    return PLATFORMINFO_ERROR;
  }

  DrvCtxt->PlatformInfo.PlatformType = Smem->PlatformType;
  DrvCtxt->PlatformInfo.Version      = Smem->PlatformVersion;
  DrvCtxt->PlatformInfo.Subtype      = Smem->PlatformSubtype;
  DrvCtxt->PlatformInfo.Fusion       = Smem->Fusion;

  if (Smem->Format >= PLATFORMINFO_VERSION (0, 18)) {
    DrvCtxt->NumKvps = Smem->NumKvps;
    if (DrvCtxt->NumKvps > 0) {
      Size          = sizeof (PlatformInfoKvpsType) * DrvCtxt->NumKvps;
      DrvCtxt->Kvps = AllocatePages (EFI_SIZE_TO_PAGES (Size));
      if (!DrvCtxt->Kvps) {
        return PLATFORMINFO_ERROR;
      }

      Kvps = (PlatformInfoKvpsType *)((UINTN *)Smem + Smem->KvpsOffset);
      CopyMem (DrvCtxt->Kvps, Kvps, Size);
    }
  }

  DrvCtxt->Initialized = TRUE;
  return PLATFORMINFO_SUCCESS;
}
