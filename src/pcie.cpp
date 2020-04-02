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

#include "pcie.h"
#include "fflog.h"

PCIE::PCIE(sc_core::sc_module_name mod_name)
    : qemu_proxy(NULL), fmod_interface(NULL) {
    owned_dbus_id = g_bus_own_name(G_BUS_TYPE_SESSION,
                                   FMOD_DBUS_NAME,
                                   G_BUS_NAME_OWNER_FLAGS_NONE,
                                   NULL,
                                   PCIE::on_name_acquired,
                                   PCIE::on_name_lost,
                                   this,
                                   NULL);
    watched_dbus_id = g_bus_watch_name(G_BUS_TYPE_SESSION,
                                       QEMU_DBUS_NAME,
                                       G_BUS_NAME_WATCHER_FLAGS_NONE,
                                       PCIE::on_name_appeared,
                                       PCIE::on_name_vanished,
                                       this,
                                       NULL);

    fep2pcie_t_socket.register_b_transport(this, &PCIE::fep2pcie_b_transport);
}

void PCIE::fep2pcie_b_transport(tlm::tlm_generic_payload &trans,
                              sc_core::sc_time &delay)
{
  
}

PCIE::~PCIE() {
    g_bus_unown_name(owned_dbus_id);
    g_bus_unwatch_name(watched_dbus_id);
}

void PCIE::on_name_appeared(GDBusConnection *connection,
                            const gchar *name,
                            const gchar *name_owner,
                            gpointer udata) {
    fflog(sc_core::SC_INFO, "FEP", "(%s) is connected", name);
}
void PCIE::on_name_vanished(GDBusConnection *connection,
                            const gchar *name,
                            gpointer udata) {
    fflog(sc_core::SC_INFO, "FEP", "(%s) is disconnected", name);
}

void PCIE::on_name_acquired(GDBusConnection *connection,
                            const gchar *name,
                            gpointer udata) {
    PCIE *pcie = static_cast<PCIE *>(udata);
    GError *error = NULL;

    pcie->fmod_interface = ffgpu_fmod_skeleton_new();
    assert(pcie->fmod_interface);

    g_signal_connect(pcie->fmod_interface,
                     "handle-read-device-memory",
                     G_CALLBACK(PCIE::handle_read_device_memory),
                     udata);
    g_signal_connect(pcie->fmod_interface,
                     "handle-write-device-memory",
                     G_CALLBACK(PCIE::handle_write_device_memory),
                     udata);

    // connection the signal for handle_xxx
    g_dbus_interface_skeleton_export(
        G_DBUS_INTERFACE_SKELETON(pcie->fmod_interface),
        connection,
        "/",
        &error);
    assert(NULL == error);

    ffgpu_qemu_proxy_new_for_bus(G_BUS_TYPE_SESSION,
                                 G_DBUS_PROXY_FLAGS_NONE,
                                 QEMU_DBUS_NAME,
                                 "/",
                                 NULL,
                                 PCIE::new_proxy_cb,
                                 udata);

    fflog(sc_core::SC_INFO, "FEP", "export DBus name: %s", FMOD_DBUS_NAME);
}

void PCIE::new_proxy_cb(GObject *source_obj,
                        GAsyncResult *res,
                        gpointer udata) {
    PCIE *pcie = static_cast<PCIE *>(udata);
    GError *error = NULL;

    pcie->qemu_proxy = ffgpu_qemu_proxy_new_for_bus_finish(res, &error);
    assert(NULL == error);
}

void PCIE::on_name_lost(GDBusConnection *connection,
                        const gchar *name,
                        gpointer udata) {
    if (!connection) {
        fflog(sc_core::SC_WARNING, "FEP", "DBus disconnected!");
    } else {
        fflog(sc_core::SC_INFO, "FEP", "ffgpu-fmod already running");
        // the instance is already running so we should exit.
        sc_core::sc_stop();
    }
}

gboolean PCIE::handle_read_device_memory(ffgpuFmod *object,
                                         GDBusMethodInvocation *invocation,
                                         guint64 arg_address,
                                         guint64 arg_size) {

    return TRUE;
}

gboolean PCIE::handle_write_device_memory(ffgpuFmod *object,
                                          GDBusMethodInvocation *invocation,
                                          guint64 arg_address,
                                          guint64 arg_size,
                                          const gchar *arg_data) {

    return TRUE;
}
