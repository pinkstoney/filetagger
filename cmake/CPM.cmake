# SPDX-License-Identifier: MIT
#
# SPDX-FileCopyrightText: Copyright (c) 2016-2023 

set(CPM_DOWNLOAD_VERSION 0.39.0)
set(CPM_HASH_SUM "66639bcac9dd2907b2918de466783554c1334446b9874e90d38e3778d404c2ef")

if(CPM_SOURCE_CACHE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

file(DOWNLOAD
     https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
     ${CPM_DOWNLOAD_LOCATION} EXPECTED_HASH SHA256=${CPM_HASH_SUM}
)

include(${CPM_DOWNLOAD_LOCATION})

# **Remove Manual FetchContent Calls**
# These manual calls interfere with CPM's handling of dependencies and should be removed.
# 
# // ... existing code ...
# FetchContent_MakeAvailable(doctest)
# FetchContent_MakeAvailable(another_dependency)
# // ... existing code ...

# ... rest of the existing code ...
