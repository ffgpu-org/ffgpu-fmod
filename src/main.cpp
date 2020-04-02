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

#include <getopt.h>
#include <glib.h>

#include <systemc>

#include "pcie.h"
#include "dc.h"
#include "dram.h"
#include "fep.h"

using namespace sc_core;

SC_MODULE(TOP) {
    SC_CTOR(TOP) {
        pcie = new PCIE("pcie");
        dram = new DRAM("dram");
        dc = new DC("dc");
        fep = new FEP("fep");

        dc->dc2dram_i_socket.bind(dram->dc2dram_t_socket);

        fep->fep2pcie_i_socket.bind(pcie->fep2pcie_t_socket);

        pcie->pcie2fep_i_socket.bind(fep->pcie2fep_t_socket);
        pcie->pcie2dc_i_socket.bind(dc->pcie2dc_t_socket);
        pcie->pcie2dram_i_socket.bind(dram->pcie2dram_t_socket);

        SC_METHOD(dut_loop);
        sensitive << clk.posedge_event();
    };

    ~TOP() {
        delete dc;
        delete dram;
        delete fep;
    };

    void dut_loop(void) {
        g_main_context_iteration(NULL, TRUE);
    };

    DC *dc;
    DRAM *dram;
    PCIE *pcie;
    FEP *fep;
    sc_clock clk;
};

int sc_main(int argc, char *argv[]) {
    TOP *top = new TOP("top");
    static struct option ffgpu_opts[] = {
        {"host-dbus-name", required_argument, 0, 0},
        {"fmod-dbus-name", required_argument, 0, 0},
        {0, 0, 0, 0}};

    sc_start(0, SC_NS);
    sc_start();

    delete top;
    return 0;
}
