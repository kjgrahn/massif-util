/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "snapshot.h"

#include "split.h"

#include <algorithm>

Snapshot::Snapshot(const std::string& time)
    : time{time}
{}

namespace {

    unsigned indent(const std::string& s)
    {
	const auto a = begin(s);
	const auto b = end(s);
	auto c = a;
	while (c != b && *c==' ') c++;
	return c - a;
    }

    bool starts_with(const std::string& haystack,
                     const std::string& needle)
    {
        auto i = std::search(begin(haystack), end(haystack),
                             begin(needle), end(needle));
        return i==begin(haystack);
    }
}

void Snapshot::add(const std::string& s)
{
    const auto v = split(s, 4);
    if (v.size() < 4) return;
    const auto& nn = v[0];
    const auto& sz = v[1];
    const auto& addr = v[2];
    const unsigned level = indent(s);
    if (!starts_with(nn, "n")) return;
}
