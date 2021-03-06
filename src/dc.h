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
    DC(sc_core::sc_module_name mod_name);
    enum REG {
        CRTC_ADDR = 1,

    };

    void init(void);

    tlm_utils::simple_initiator_socket<DC> dc2dram_i_socket;
    tlm_utils::simple_target_socket<DC> pcie2dc_t_socket;

    struct {
        uint32_t CRTC_ADDR; // start location of framebuffer
    } reg;

    GtkWidget *window;
    gint win_width;
    gint win_height;

    static gboolean
    on_delete(GtkWidget *widget, GdkEvent *event, gpointer user_data);
    static gboolean on_draw(GtkWidget *wdg, cairo_t *cr, gpointer udata);
    static gboolean do_refresh(gpointer udata);
    void pcie2dc_b_transport(tlm::tlm_generic_payload &trans,
                             sc_core::sc_time &delay);
};

// Local Variables:
// mode: c++
// End:
