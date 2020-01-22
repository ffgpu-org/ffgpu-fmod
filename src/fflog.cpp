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

#include "fflog.h"

void fflog(sc_core::sc_severity level, const char *msg_type, const char *msg_fmt, ...)
{
  va_list ap;
  gchar *msg_str;

  va_start(ap, msg_fmt);
  msg_str = g_strdup_vprintf(msg_fmt, ap);
  va_end(ap);

  switch (level) {
  case sc_core::SC_INFO:
    SC_REPORT_INFO(msg_type, msg_str);
    break;
  case sc_core::SC_WARNING:
    SC_REPORT_WARNING(msg_type, msg_str);
    break;
  case sc_core::SC_ERROR:
    SC_REPORT_ERROR(msg_type, msg_str);
    break;
  case sc_core::SC_FATAL:
    SC_REPORT_FATAL(msg_type, msg_str);
    break;
  case sc_core::SC_MAX_SEVERITY:
  default:
    break;
  }

  g_free(msg_str);
}
