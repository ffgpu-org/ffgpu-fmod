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

#include "host.h"
#include "fflog.h"
#include "regdef.h"

HOST::HOST(sc_core::sc_module_name mod_name) :
  fmod_interface(NULL),
  qemu_proxy(NULL)
{
  owned_dbus_id = g_bus_own_name(G_BUS_TYPE_SESSION, FMOD_DBUS_NAME, G_BUS_NAME_OWNER_FLAGS_NONE, NULL,
				   HOST::on_name_acquired, HOST::on_name_lost, this, NULL);
  watched_dbus_id = g_bus_watch_name(G_BUS_TYPE_SESSION, QEMU_DBUS_NAME, G_BUS_NAME_WATCHER_FLAGS_NONE,
				     HOST::on_name_appeared, HOST::on_name_vanished, this, NULL);
};

HOST::~HOST()
{
    
}

void HOST::on_name_appeared(GDBusConnection *connection,
			    const gchar *name,
			    const gchar *name_owner, gpointer udata)
{
  fflog(sc_core::SC_INFO, "HOST", "(%s) is connected", name);
}
void HOST::on_name_vanished(GDBusConnection *connection,
			    const gchar *name, gpointer udata)
{
  fflog(sc_core::SC_INFO, "HOST", "(%s) is disconnected", name);
}


void HOST::new_proxy_cb(GObject *source_obj, GAsyncResult *res, gpointer udata)
{
  HOST *host = (HOST *)udata;
  GError *error = NULL;

  host->qemu_proxy = ffgpu_qemu_proxy_new_for_bus_finish(res, &error);
  assert(NULL == error);
}

void HOST::on_name_acquired(GDBusConnection *connection, const gchar *name,
			    gpointer udata)
{
  HOST *host = (HOST *)udata;
  GError *error = NULL;

  host->fmod_interface = ffgpu_fmod_skeleton_new();
  assert(host->fmod_interface);

  // connection the signal for handle_xxx

  g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(host->fmod_interface),
				   connection, "/", &error);
  assert(NULL == error);
  
  ffgpu_qemu_proxy_new_for_bus(G_BUS_TYPE_SESSION,
			       G_DBUS_PROXY_FLAGS_NONE,
			       QEMU_DBUS_NAME,
			       "/",
			       NULL,
			       HOST::new_proxy_cb,
			       udata);

  fflog(sc_core::SC_INFO, "HOST", "export DBus name: %s", FMOD_DBUS_NAME);
}

void HOST::on_name_lost(GDBusConnection *connection, const gchar *name,
                          gpointer udata)
{
  if (!connection) {
      fflog(sc_core::SC_WARNING, "HOST", "DBus disconnected!");
  } else {
    fflog(sc_core::SC_INFO, "HOST", "ffgpu-fmod already running");
    
    // the instance is already running so we should exit.
    sc_core::sc_stop();
  }
}
