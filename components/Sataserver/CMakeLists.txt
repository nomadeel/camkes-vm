#
# Copyright 2018, Data61, CSIRO (ABN 41 687 119 230)
# Copyright 2019, DornerWorks
#
# SPDX-License-Identifier: BSD-2-Clause
#
cmake_minimum_required(VERSION 3.8.2)

project(Sataserver C)

if(LibSatadriversAHCIEnable)
    if("${CAmkESVMDestHardware}" STREQUAL "")
        message(
            FATAL_ERROR
                "libsatadrivers is using AHCI but CAmkESVMDestHardware is not set, cannot find hardware details"
        )
    endif()
    CAmkESAddCPPInclude(
        "${CMAKE_CURRENT_LIST_DIR}/include/plat/${KernelPlatform}/${CAmkESVMDestHardware}"
    )
else()
    # Add the empty sata definitions file
    CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/empty")
endif()

# Find header with hardware details, can find the empty
DeclareCAmkESComponent(
    Sataserver
    SOURCES
    src/sataserver.c
    INCLUDES
    include
    LIBS
    sel4allocman
    sel4platsupport
    satadrivers
)
