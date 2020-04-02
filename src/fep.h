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
#include <gio/gio.h>
#include <glib.h>
#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include "conf.h"
#include "generated-fmod-intf.h"
#include "generated-qemu-intf.h"

class FEP : public sc_core::sc_module {
  public:
    SC_HAS_PROCESS(FEP);
    FEP(sc_core::sc_module_name mod_name);

    ~FEP();

    tlm_utils::simple_initiator_socket<FEP> fep2pcie_i_socket;
    tlm_utils::simple_target_socket<FEP> pcie2fep_t_socket;

  private:
    void pcie2fep_b_transport(tlm::tlm_generic_payload &trans,
                              sc_core::sc_time &delay);
};

// Local Variables:
// mode: c++
// End:
