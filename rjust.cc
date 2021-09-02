/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "rjust.h"

#include <iostream>
#include <cstring>

rjust::rjust(unsigned width, const std::string& val)
    : width{width},
      val{val}
{}

std::ostream& operator<< (std::ostream& os, const rjust& val)
{
    char buf[20];
    std::snprintf(buf, sizeof buf,
		  "%*s", val.width, val.val.c_str());
    return os << buf;
}
