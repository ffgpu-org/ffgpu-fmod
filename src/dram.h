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
#include <systemc>
#include <tlm_utils/simple_target_socket.h>

SC_MODULE(DRAM) {

 public:
    SC_CTOR(DRAM) {
      pmem = map_dram();
      assert(pmem);

      dc_dram_t_socket.register_b_transport(this, &DRAM::b_transport);
      dc_dram_t_socket.register_get_direct_mem_ptr(this, &DRAM::get_direct_mem_ptr);

      host_dram_t_socket.register_b_transport(this, &DRAM::b_transport);
      host_dram_t_socket.register_get_direct_mem_ptr(this, &DRAM::get_direct_mem_ptr);
    };
    ~DRAM();

    tlm_utils::simple_target_socket<DRAM> dc_dram_t_socket;
    tlm_utils::simple_target_socket<DRAM> host_dram_t_socket;
    
 private:
    void *map_dram(void);
    void *pmem;
    int fd_shm;

    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
};




















// Local Variables:
// mode: c++
// End:
