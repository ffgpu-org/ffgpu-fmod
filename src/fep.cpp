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

#include "fep.h"
#include "fflog.h"
#include "regdef.h"

FEP::FEP(sc_core::sc_module_name mod_name) {
  pcie2fep_t_socket.register_b_transport(this, &FEP::pcie2fep_b_transport);
};

FEP::~FEP() {
}

void FEP::pcie2fep_b_transport(tlm::tlm_generic_payload &trans,
                              sc_core::sc_time &delay)
{
  
}
