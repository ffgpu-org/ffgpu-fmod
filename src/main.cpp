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

#include <glib.h>
#include <systemc>
#include "host.h"
#include "dram.h"
#include "dc.h"

using namespace sc_core;

SC_MODULE(TOP) {

  SC_CTOR(TOP) {
    dram = new DRAM("dram");
    dc = new DC("dc");
    dc->init();
    dc->dc_dram_i_socket.bind(dram->dc_dram_t_socket);
    
    host = new HOST("host");
    host->host_dram_i_socket.bind(dram->host_dram_t_socket);
    host->host_dc_i_socket.bind(dc->host_dc_t_socket);
    
    SC_METHOD(dut_loop);
    sensitive << clk.posedge_event();
  };

  ~TOP() {
    delete dc;
    delete dram;
    delete host;
  };

  void dut_loop(void) {
    g_main_context_iteration(NULL, TRUE);
  };

  DC *dc;
  DRAM *dram;
  HOST *host;
  sc_clock clk;
  
};

int sc_main(int argc, char * argv[])
{
  TOP *top = new TOP("top");

  sc_start(0, SC_NS);
  sc_start();

  delete top;
  return 0;
}
