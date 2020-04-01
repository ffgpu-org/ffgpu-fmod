###############################################################################
# ____________ _____ ______ _   _ 
# |  ___|  ___|  __ \| ___ \ | | |
# | |_  | |_  | |  \/| |_/ / | | |
# |  _| |  _| | | __ |  __/| | | |
# | |   | |   | |_\ \| |   | |_| |
# \_|   \_|    \____/\_|    \___/ 
#                                
# Copyright (C) 2017-2020 ffgpu.org
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation;
#
###############################################################################

cmake_minimum_required (VERSION 3.10)
project (ffgpu-fmod)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option(FFGPU_QEMU_DBUS_XML "The ffgpu-qemu dbus interface xml file" "")
if(NOT FFGPU_QEMU_DBUS_XML)
  message(FATAL_ERROR "Please set the qemu dbus xml file: FFGPU_QEMU_DBUS_XML")
endif()
if(NOT IS_ABSOLUTE ${FFGPU_QEMU_DBUS_XML})
  set(FFGPU_QEMU_DBUS_XML "${CMAKE_CURRENT_SOURCE_DIR}/${FFGPU_QEMU_DBUS_XML}")
endif()
if(NOT EXISTS ${FFGPU_QEMU_DBUS_XML})
  message(FATAL_ERROR "file ${FFGPU_QEMU_DBUS_XML} not exsit")
endif()

message(STATUS "ffgpu-qemu dbus interface xml: ${FFGPU_QEMU_DBUS_XML}")

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK gtk+-3.0 REQUIRED)
pkg_check_modules (SYSTEMC systemc REQUIRED)
pkg_check_modules (TLM tlm REQUIRED)

set (SRCS
  ${CMAKE_CURRENT_BINARY_DIR}/generated-fmod-intf.c
  ${CMAKE_CURRENT_BINARY_DIR}/generated-qemu-intf.c
  src/main.cpp
  src/dc.cpp
  src/dram.cpp
  src/fflog.cpp
  src/host.cpp
  )

link_directories(BEFORE ${SYSTEMC_LIBRARY_DIRS})
add_executable (ffgpu-fmod ${SRCS})
target_include_directories (ffgpu-fmod BEFORE PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${GTK_INCLUDE_DIRS} )
target_link_libraries (ffgpu-fmod  ${GTK_LIBRARIES} ${SYSTEMC_LIBRARIES} ${TLM_LIBRARIES} rt)


add_custom_command(OUTPUT generated-fmod-intf.c generated-fmod-intf.h
  COMMAND gdbus-codegen --generate-c-code generated-fmod-intf --c-namespace FFgpu --interface-prefix org.ffgpu 
  ${CMAKE_CURRENT_SOURCE_DIR}/org.ffgpu.fmod.xml )
add_custom_command(OUTPUT generated-qemu-intf.c generated-qemu-intf.h
  COMMAND gdbus-codegen --generate-c-code generated-qemu-intf --c-namespace FFgpu --interface-prefix org.ffgpu 
  ${FFGPU_QEMU_DBUS_XML} )

add_custom_target(gen_dbus DEPENDS generated-fmod-intf.c generated-fmod-intf.h generated-qemu-intf.c generated-qemu-intf.h)
add_dependencies(ffgpu-fmod gen_dbus)
