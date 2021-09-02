/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef MASSIF_RJUST_H
#define MASSIF_RJUST_H

#include <string>
#include <iosfwd>

struct rjust {
    rjust(unsigned width, const std::string& val);
    const unsigned width;
    const std::string& val;
};

std::ostream& operator<< (std::ostream& os, const rjust& val);

#endif
