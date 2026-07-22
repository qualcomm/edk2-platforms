/** @file
  Internal header file for the PlatformInfo driver.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

#include <Base.h>
#include <Library/PlatformInfoLib.h>

/*=========================================================================
      Type Definitions
==========================================================================*/

typedef struct {
  UINT32    nKey;
  UINT32    nValue;
} PlatformInfoKVPSType;

/*
 * PlatformInfo driver context.
 */
typedef struct PlatformInfoDrvCtxt {
  BOOLEAN                         bInitialized;
  PlatformInfoPlatformInfoType    PlatformInfo;
  UINT32                          nNumKVPS;
  PlatformInfoKVPSType            *aKVPS;
} PlatformInfoDrvCtxt;

/*=========================================================================
      Function Definitions
==========================================================================*/

PlatformInfoResult
PlatformInfo_GetPropertyValue (
  const char  *szName,
  void        **pValue
  );

PlatformInfoDrvCtxt *
PlatformInfo_GetDrvCtxt (
  );
