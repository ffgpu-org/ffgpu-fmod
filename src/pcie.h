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

class PCIE : public sc_core::sc_module {
  public:
    SC_HAS_PROCESS(PCIE);
    PCIE(sc_core::sc_module_name mod_name);
    ~PCIE();

  tlm_utils::simple_initiator_socket<PCIE> pcie2fep_i_socket;
  tlm_utils::simple_initiator_socket<PCIE> pcie2dc_i_socket;
  tlm_utils::simple_initiator_socket<PCIE> pcie2dram_i_socket;
  tlm_utils::simple_target_socket<PCIE> fep2pcie_t_socket;

  private:
    guint owned_dbus_id;
    guint watched_dbus_id;
    ffgpuFmod *fmod_interface;
    ffgpuQemu *qemu_proxy;

    static void on_name_acquired(GDBusConnection *connection,
                                 const gchar *name,
                                 gpointer udata);
    static void on_name_lost(GDBusConnection *connection,
                             const gchar *name,
                             gpointer udata);

    static void on_name_appeared(GDBusConnection *connection,
                                 const gchar *name,
                                 const gchar *name_owner,
                                 gpointer udata);
    static void on_name_vanished(GDBusConnection *connection,
                                 const gchar *name,
                                 gpointer udata);
    static void
    new_proxy_cb(GObject *source_obj, GAsyncResult *res, gpointer udata);

    static gboolean handle_read_device_memory(ffgpuFmod *object,
                                              GDBusMethodInvocation *invocation,
                                              guint64 arg_address,
                                              guint64 arg_size);

    static gboolean
    handle_write_device_memory(ffgpuFmod *object,
                               GDBusMethodInvocation *invocation,
                               guint64 arg_address,
                               guint64 arg_size,
                               const gchar *arg_data);


   void fep2pcie_b_transport(tlm::tlm_generic_payload &trans,
                              sc_core::sc_time &delay);
};

// Local Variables:
// mode: c++
// End:
