/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "snapshot.h"

#include "split.h"
#include "graph.h"
#include "md5pp.h"
#include "rjust.h"

#include <algorithm>
#include <iostream>

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

    bool contains(const std::string& haystack,
		  const std::string& needle)
    {
        auto i = std::search(begin(haystack), end(haystack),
                             begin(needle), end(needle));
        return i!=end(haystack);
    }

    std::string cleanup(const std::string& addr)
    {
	if (addr=="(heap") return "0";
	if (starts_with(addr, "0x")) return {begin(addr)+2, end(addr)-1};
	return addr;
    }

    md5::Digest md5sum(const std::string& s)
    {
	return md5::Ctx().update(s).digest();
    }
}

void Snapshot::add(const std::string& s)
{
    const auto v = split(s, 4);
    if (v.size() < 4) return;
    const auto& nn = v[0];
    const auto& sz = v[1];
    auto addr = v[2];
    const unsigned level = indent(s);
    if (!starts_with(nn, "n")) return;
    if (contains(v[3], "below massif's threshold")) addr = "bits";

    stack.resize(level);
    stack.push_back(cleanup(addr));

    if (nn=="n0:") {
	ee.emplace_back(sz, join('/', stack));
    }
}

void Snapshot::put(std::ostream& os) const
{
    for (const auto& e : ee) {
	os << rjust(12, time) << ' '
	   << rjust(9, e.size) << ' '
	   << md5sum(e.path) << ' '
	   << e.path << '\n';
    }
}

void Snapshot::put(Graph& graph) const
{
    for (const auto& e : ee) {
	graph.add(split("/", e.path));
    }
}
