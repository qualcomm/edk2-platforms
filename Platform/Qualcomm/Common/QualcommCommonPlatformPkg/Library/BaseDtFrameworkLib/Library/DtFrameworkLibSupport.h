/** @file

  Library routine required for DTFramework.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma once

#include <Uefi.h>
#include <Library/DebugLib.h>

/**
  Returns the current performance counter value in microseconds.

  Retrieves the performance counter frequency on the first call and caches
  it for subsequent calls. Validates that the frequency is within an
  acceptable range and that the counter is an up-counter before performing
  the conversion.

  @retval  0    The frequency is invalid or the counter is not supported.
  @retval  !0   The current performance counter value in microseconds.
**/
UINT64
GetTimerCountus (
  VOID
  );
