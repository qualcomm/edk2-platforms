/** @file

  Interface implementation file for the PlatformInfo driver.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries. All rights reserved.
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
PlatformInfo_Init (
  VOID
  )
{
  PlatformInfoDrvCtxt   *pDrvCtxt;
  UINT32                nSize;
  PlatformInfoSMemType  *pSMem;
  PlatformInfoKVPSType  *pKVPS;

  pDrvCtxt = PlatformInfo_GetDrvCtxt ();

  if (pDrvCtxt->bInitialized) {
    return PLATFORMINFO_SUCCESS;
  }

  pSMem = SmemGetAddr (SmemHwSwBuildId, &nSize);
  if (pSMem == NULL) {
    return PLATFORMINFO_ERROR;
  }

  pDrvCtxt->PlatformInfo.platform = pSMem->ePlatformType;
  pDrvCtxt->PlatformInfo.version  = pSMem->nPlatformVersion;
  pDrvCtxt->PlatformInfo.subtype  = pSMem->nPlatformSubtype;
  pDrvCtxt->PlatformInfo.fusion   = pSMem->bFusion;

  if (pSMem->nFormat >= PLATFORMINFO_VERSION (0, 18)) {
    pDrvCtxt->nNumKVPS = pSMem->nNumKVPS;
    if (pDrvCtxt->nNumKVPS > 0) {
      nSize           = sizeof (PlatformInfoKVPSType) * pDrvCtxt->nNumKVPS;
      pDrvCtxt->aKVPS = AllocatePages (EFI_SIZE_TO_PAGES (nSize));
      if (!pDrvCtxt->aKVPS) {
        return PLATFORMINFO_ERROR;
      }

      pKVPS = (PlatformInfoKVPSType *)((UINTN *)pSMem + pSMem->nKVPSOffset);
      CopyMem (pDrvCtxt->aKVPS, pKVPS, nSize);
    }
  }

  pDrvCtxt->bInitialized = TRUE;
  return PLATFORMINFO_SUCCESS;
}
