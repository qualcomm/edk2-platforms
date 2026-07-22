/** @file
  Image-specific implementations of external functions used by
  ChipInfo which are accessed using different APIs on each image

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/SmemLib.h>
#include "ChipInfoImage.h"
#include "PlatformInfoDefs.h"

/**
  Get the SoC information from SMEM.

  Common way to access the socinfo SMEM region, since not all images
  have an SMEM driver for accessing socinfo.

  @retval  Non-NULL  Pointer to the PlatformInfoSMemType structure in SMEM.
  @retval  NULL      SMEM address is unavailable or the region size is zero.

**/
PlatformInfoSMemType *
ChipInfo_GetSocInfo (
  VOID
  )
{
  PlatformInfoSMemType  *pSMem;
  UINT32                nSize;

  pSMem = (PlatformInfoSMemType *)SmemGetAddr (SmemHwSwBuildId, &nSize);
  if ((pSMem == NULL) || (nSize == 0)) {
    return NULL;
  }

  return pSMem;
}

/**
  Unmap the SMEM region once it is no longer needed.

  Releases any mapping obtained by ChipInfo_GetSocInfo. On this image,
  no unmapping is required.

**/
VOID
ChipInfo_UnmapSMem (
  VOID
  )
{
  // Nothing to unmap
  return;
}
