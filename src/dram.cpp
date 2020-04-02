///////////////////////////////////////////////////////////////////////////////
// ____________ _____ ______ _   _
// |  ___|  ___|  __ \| ___ \ | | |
// | |_  | |_  | |  \/| |_/ / | | |
// |  _| |  _| | | __ |  __/| | | |
// | |   | |   | |_\ \| |   | |_| |
// \_|   \_|    \____/\_|    \___/
//
// Copyright (C) 2017-2020 ffgpu.org
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// version 2 as published by the Free Software Foundation;
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <glib.h>
#include "fflog.h"
#include "conf.h"
#include "dram.h"

DRAM::DRAM(sc_core::sc_module_name mod_name) {
    pmem = ::malloc(DRAM_SIZE);
    assert(pmem);

    pcie2dram_t_socket.register_b_transport(this, &DRAM::dram_b_transport);
    dc2dram_t_socket.register_get_direct_mem_ptr(
        this, &DRAM::dram_get_direct_mem_ptr);
}

DRAM::~DRAM() {
    free(pmem);
}

void DRAM::dram_b_transport(tlm::tlm_generic_payload &trans,
                            sc_core::sc_time &delay) {
    tlm::tlm_command cmd = trans.get_command();
    uint64_t addr = trans.get_address();
    uint8_t *ptr = trans.get_data_ptr();
    uint8_t *byt = trans.get_byte_enable_ptr();
    uint32_t len = trans.get_data_length();
    uint32_t sw = trans.get_streaming_width();

    if (byt != NULL || sw != len) {
        assert(false);
    }

    if (cmd == tlm::TLM_READ_COMMAND) {

        ::memcpy(ptr, (void *)((uintptr_t)pmem + addr), len);

    } else if (cmd == tlm::TLM_WRITE_COMMAND) {

        ::memcpy((void *)((uintptr_t)pmem + addr), ptr, len);

    } else if (cmd == tlm::TLM_IGNORE_COMMAND) {
        // ignore it
    } else {
        assert(false);
    }

    trans.set_dmi_allowed(true);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

bool DRAM::dram_get_direct_mem_ptr(tlm::tlm_generic_payload &trans,
                                   tlm::tlm_dmi &dmi_data) {
    dmi_data.allow_read_write();
    dmi_data.set_dmi_ptr((unsigned char *)pmem + dmi_data.get_start_address());

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    return true;
}
