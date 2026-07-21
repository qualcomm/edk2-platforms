/** @file
  GetTimerCountus implementation - required for DTFramework.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

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
  )
{
  STATIC UINT64   CachedFrequency    = 0;
  STATIC UINT32   FactorMicroSeconds = 0;
  STATIC BOOLEAN  Initialized        = FALSE;
  UINT64          TimeTicks;
  UINT64          StartValue;
  UINT64          EndValue;

  if (!Initialized) {
    CachedFrequency = GetPerformanceCounterProperties (&StartValue, &EndValue);

    //
    // Validate frequency is reasonable (> 1 MHz)
    //
    if (CachedFrequency <= 1000000ULL) {
      DEBUG ((DEBUG_ERROR, "%a: Frequency too low\n", __func__));
      return 0;
    }

    //
    // Validate frequency does not exceed 32-bit range (< 4 GHz)
    //
    if (CachedFrequency >= 0x100000000ULL) {
      DEBUG ((DEBUG_ERROR, "%a: Frequency too high\n", __func__));
      return 0;
    }

    //
    // Validate counter counts up (not down)
    //
    if (StartValue >= EndValue) {
      DEBUG ((DEBUG_ERROR, "%a: Down-counter not supported\n", __func__));
      return 0;
    }

    //
    // Pre-calculate conversion factor: Frequency / 1,000,000
    // Avoids overflow in the main calculation path
    //
    FactorMicroSeconds = (UINT32)DivU64x32 (CachedFrequency, 1000000UL);

    Initialized = TRUE;
  }

  //
  // Convert ticks to microseconds:
  //   TimeUs = TimeTicks / (Frequency / 1,000,000)
  // Equivalent to (TimeTicks * 1,000,000) / Frequency but avoids overflow
  //
  TimeTicks = GetPerformanceCounter ();

  return DivU64x32 (TimeTicks, FactorMicroSeconds);
}
