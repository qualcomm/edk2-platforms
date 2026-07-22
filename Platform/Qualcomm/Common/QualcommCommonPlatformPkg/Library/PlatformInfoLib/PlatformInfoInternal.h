/** @file
  Internal header file for the PlatformInfo driver.

  @par Glossary:
    - Kvps - Key-Value PairS

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

#include <Base.h>
#include <Library/PlatformInfoLib.h>

/*=========================================================================
      Type Definitions
==========================================================================*/

typedef struct {
  UINT32    Key;
  UINT32    Value;
} PlatformInfoKvpsType;

/*
 * PlatformInfo driver context.
 */
typedef struct {
  BOOLEAN                         Initialized;
  PlatformInfoPlatformInfoType    PlatformInfo;
  UINT32                          NumKvps;
  PlatformInfoKvpsType            *Kvps;
} PlatformInfoDrvCtxt;

/*=========================================================================
      Function Definitions
==========================================================================*/

PlatformInfoResult
EFIAPI
PlatformInfoGetPropertyValue (
  IN  CONST CHAR8  *Name,
  OUT VOID          **Value
  );

PlatformInfoDrvCtxt *
EFIAPI
PlatformInfoGetDrvCtxt (
  VOID
  );
