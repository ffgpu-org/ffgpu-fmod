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
#include <gtk/gtk.h>
#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>


class DC : public sc_core::sc_module {

public:
  SC_HAS_PROCESS(DC);
  DC(sc_core::sc_module_name mod_name) :
    win_width(1024),
    win_height(768)
  {
    // g_timeout_add(1000/60, do_refresh, this);
    host_dc_t_socket.register_b_transport(this, &DC::b_transport);

    reg.FB_START_ADDR = 0;
  };

  bool init(void);

  tlm_utils::simple_initiator_socket<DC> dc_dram_i_socket;
  tlm_utils::simple_target_socket<DC> host_dc_t_socket;

  struct {
    uint32_t FB_START_ADDR;    // start location of framebuffer
  } reg;

  GtkWidget *window;
  gint win_width;
  gint win_height;

  static gboolean on_delete(GtkWidget *widget, GdkEvent *event, gpointer user_data);
  static gboolean on_draw(GtkWidget *wdg, cairo_t *cr, gpointer udata);
  static gboolean do_refresh(gpointer udata);
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};




// Local Variables:
// mode: c++
// End:
