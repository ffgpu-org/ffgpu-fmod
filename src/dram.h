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

#pragma once
#include "conf.h"
#include <systemc>
#include <tlm_utils/simple_target_socket.h>

SC_MODULE(DRAM) {

  public:
    SC_HAS_PROCESS(DRAM);
    DRAM(sc_core::sc_module_name mod_name);
    ~DRAM();

    tlm_utils::simple_target_socket<DRAM> pcie2dram_t_socket;
    tlm_utils::simple_target_socket<DRAM> dc2dram_t_socket;

  private:
    void *pmem;

    void dram_b_transport(tlm::tlm_generic_payload & trans,
                          sc_core::sc_time & delay);
    bool dram_get_direct_mem_ptr(tlm::tlm_generic_payload & trans,
                                 tlm::tlm_dmi & dmi_data);
};

// Local Variables:
// mode: c++
// End:
