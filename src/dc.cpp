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

#include "dc.h"
#include "fflog.h"
#include "regdef.h"

using namespace std;
using namespace sc_core;
using namespace tlm_utils;

DC::DC(sc_core::sc_module_name mod_name) : win_width(1024), win_height(768) {

    pcie2dc_t_socket.register_b_transport(this, &DC::pcie2dc_b_transport);

    reg.CRTC_ADDR = 0;

    init();
};

gboolean DC::do_refresh(gpointer udata) {
    DC *dc = (DC *)udata;

    gtk_widget_queue_draw(GTK_WIDGET(dc->window));

    return TRUE;
}

void DC::init(void) {
    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    assert(window);

    gtk_window_set_default_size(GTK_WINDOW(window), win_width, win_height);

    /* user can't resize window */
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(
        G_OBJECT(window), "destroy", G_CALLBACK(DC::on_delete), this);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(DC::on_draw), this);

    gtk_widget_show_all(window);

    // the main_loop is in the TOP
}

gboolean DC::on_delete(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    sc_stop();
    return FALSE;
}

gboolean DC::on_draw(GtkWidget *widget, cairo_t *cr, gpointer udata) {
    bool can_dmi = false;
    uint8_t *pmem = NULL;
    DC *dc = (DC *)(udata);
    sc_core::sc_time delay(0, SC_NS);
    tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
    tlm::tlm_dmi *dmi = new tlm::tlm_dmi;
    GdkPixbuf *fb_pbuf;
    GBytes *fb_bytes;
    uint32_t fb_sz;

    dmi->set_start_address(dc->reg.CRTC_ADDR);
    can_dmi = dc->dc2dram_i_socket->get_direct_mem_ptr(*trans, *dmi);
    assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
    assert(can_dmi);

    pmem = dmi->get_dmi_ptr();
    assert(pmem);

    fb_sz = dc->win_width * dc->win_height * sizeof(uint32_t);

    fb_bytes = g_bytes_new(pmem, fb_sz);

    fb_pbuf = gdk_pixbuf_new_from_bytes(fb_bytes,
                                        GDK_COLORSPACE_RGB,
                                        TRUE,
                                        8,
                                        dc->win_width,
                                        dc->win_height,
                                        dc->win_width * sizeof(uint32_t));
    gdk_cairo_set_source_pixbuf(cr, fb_pbuf, 0, 0);

    cairo_paint(cr);

    g_object_unref(fb_pbuf);
    g_bytes_unref(fb_bytes);
    delete trans;
    delete dmi;
    return TRUE;
}

void DC::pcie2dc_b_transport(tlm::tlm_generic_payload &trans,
                             sc_core::sc_time &delay) {
    tlm::tlm_command cmd = trans.get_command();
    uint64_t addr = trans.get_address();
    uint8_t *ptr = trans.get_data_ptr();
    uint8_t *byt = trans.get_byte_enable_ptr();
    uint32_t len = trans.get_data_length();
    uint32_t sw = trans.get_streaming_width();

    if (byt != NULL || sw != len) {
        assert(false);
    }

    switch (addr) {
    case DC::REG::CRTC_ADDR: {
        if (cmd == tlm::TLM_READ_COMMAND) {
            if (len >= sizeof(reg.CRTC_ADDR)) {
                ::memcpy(ptr, &reg.CRTC_ADDR, sizeof(reg.CRTC_ADDR));
                trans.set_response_status(tlm::TLM_OK_RESPONSE);
            } else {
                fflog(SC_WARNING, "DC", "can't read CRTC_ADDR");
                trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            }
        } else if (cmd == tlm::TLM_WRITE_COMMAND) {
            reg.CRTC_ADDR = *(uint32_t *)ptr;
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            // we need redraw the screen
            gtk_widget_queue_draw(GTK_WIDGET(window));
        } else if (cmd == tlm::TLM_IGNORE_COMMAND) {
            fflog(SC_WARNING, "DC", "Got IGNORE command");
            trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
        }
    } break;
    default:
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        break;
    }
}

// Local Variables:
// mode: c++
// End:
