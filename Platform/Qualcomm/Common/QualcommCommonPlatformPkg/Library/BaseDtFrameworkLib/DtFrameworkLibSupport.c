/** @file
  GetTimerCountus implementation - required for DTFramework.

  Also provides fdt_strrchr()/fdt_strtoul() - ISO C strrchr/strtoul
  equivalents required by the libfdt sources bundled with DTFramework,
  since libfdt is built with -nostdinc and has no C library to draw them
  from.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2023 Pedro Falcato All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#define MICROSECONDS_PER_SECOND  1000000ULL

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
  UINT32  Factor;
  UINT64  Frequency;

  Frequency = GetPerformanceCounterProperties (NULL, NULL);
  Factor    = (UINT32)(Frequency / MICROSECONDS_PER_SECOND);
  if (Factor == 0) {
    return 0;
  }

  return DivU64x32 (GetPerformanceCounter (), Factor);
}

/**
  Returns a pointer to the last occurrence of Char in Str.

  No existing edk2 function provides ISO C strrchr() semantics (BaseLib's
  ScanMem8()/AsciiStrStr() search forwards for the first match, not the
  last); this is a local reimplementation rather than a reuse of an
  existing edk2 API.

  @param[in]  Str   Pointer to the null-terminated string to search.
  @param[in]  Char  Character to locate.

  @retval  Pointer to the last occurrence of Char in Str, or the string's
           terminating null character if Char is '\0'.
  @retval  NULL if Char does not occur in Str.
**/
CHAR8 *
fdt_strrchr (
  IN CONST CHAR8  *Str,
  IN INT32        Char
  )
{
  CHAR8  *S;
  CHAR8  *Last;

  S    = (CHAR8 *)Str;
  Last = NULL;

  for ( ; ; S++) {
    if (*S == Char) {
      Last = S;
    }

    if (*S == '\0') {
      return Last;
    }
  }
}

/**
  Checks whether Ch is an ISO C whitespace character.

  Recognizes carriage return, newline, space, tab, vertical tab, and
  form feed.

  @param[in]  Ch  Character to check.

  @retval  TRUE   Ch is a whitespace character.
  @retval  FALSE  Ch is not a whitespace character.
**/
STATIC
BOOLEAN
FdtIsSpace (
  IN INTN  Ch
  )
{
  return Ch == '\r' || Ch == '\n' || Ch == ' ' || Ch == '\t' || Ch == '\v' || Ch == '\f';
}

/**
  Converts the initial part of Nptr to an unsigned long, per ISO C strtoul
  semantics (auto-detecting base 0, 2, 8, 10 and 16 prefixes).

  No existing edk2 function reuses this: BaseLib's AsciiStrDecimalToUintn()/
  AsciiStrHexToUintn() each assume a fixed base and neither auto-detects
  0x/0b/0 prefixes the way ISO C strtoul() does, which the libfdt sources
  bundled with DTFramework rely on; this is a local reimplementation
  rather than a reuse of an existing edk2 API.

  @param[in]   Nptr    Pointer to the null-terminated string to convert.
  @param[out]  EndPtr  If not NULL, set to point to the character after the
                        last character used in the conversion.
  @param[in]   Base    Numeric base to use, or 0 to auto-detect.

  @retval  The converted value, or MAX_UINTN on overflow.
**/
UINTN
fdt_strtoul (
  IN  CONST CHAR8  *Nptr,
  OUT CHAR8        **EndPtr,
  IN  INT32        Base
  )
{
  BOOLEAN  Negate;
  BOOLEAN  Overflow;
  UINTN    Val;

  Negate   = FALSE;
  Overflow = FALSE;
  Val      = 0;

  // Reject bad numeric bases
  if ((Base < 0) || (Base == 1) || (Base > 36)) {
    return 0;
  }

  // Skip whitespace
  while (FdtIsSpace (*Nptr)) {
    Nptr++;
  }

  // Check for + or - prefixes
  if (*Nptr == '-') {
    Negate = TRUE;
    Nptr++;
  } else if (*Nptr == '+') {
    Nptr++;
  }

  // Consume the start, autodetecting base if needed
  if ((Nptr[0] == '0') && ((Nptr[1] == 'x') || (Nptr[1] == 'X')) && ((Base == 0) || (Base == 16))) {
    // Hex
    Nptr += 2;
    Base  = 16;
  } else if ((Nptr[0] == '0') && ((Nptr[1] == 'b') || (Nptr[1] == 'B')) && ((Base == 0) || (Base == 2))) {
    // Binary (standard pending C23)
    Nptr += 2;
    Base  = 2;
  } else if ((Nptr[0] == '0') && ((Base == 0) || (Base == 8))) {
    // Octal
    Nptr++;
    Base = 8;
  } else {
    if (Base == 0) {
      // Assume decimal
      Base = 10;
    }
  }

  while (TRUE) {
    INTN   Digit;
    CHAR8  C;
    UINTN  NewVal;

    C     = *Nptr;
    Digit = -1;

    if ((C >= '0') && (C <= '9')) {
      Digit = C - '0';
    } else if ((C >= 'a') && (C <= 'z')) {
      Digit = C - 'a' + 10;
    } else if ((C >= 'A') && (C <= 'Z')) {
      Digit = C - 'A' + 10;
    }

    if ((Digit == -1) || (Digit >= Base)) {
      // Note that this case also handles the '\0'
      if (EndPtr) {
        *EndPtr = (CHAR8 *)Nptr;
      }

      break;
    }

    NewVal = Val * Base + Digit;

    if (NewVal < Val) {
      // Overflow
      Overflow = TRUE;
    }

    Val = NewVal;

    Nptr++;
  }

  if (Negate) {
    Val = (UINTN)(-(INTN)Val);
  }

  if (Overflow) {
    Val = MAX_UINTN;
  }

  return Val;
}
