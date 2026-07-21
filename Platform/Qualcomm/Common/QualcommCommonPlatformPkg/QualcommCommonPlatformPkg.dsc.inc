## @file
# Qualcomm Common Platform dsc.inc file.
#
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses.Common]
  DtFrameworkLib|QualcommCommonPlatformPkg/Library/BaseDtFrameworkLib/BaseDtFrameworkLib.inf

[Components.AARCH64]
  QualcommCommonPlatformPkg/Drivers/DtbExtnDxe/DtbExtnDxe.inf
