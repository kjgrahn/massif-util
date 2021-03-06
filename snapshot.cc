/* Copyright (c) 2021 J?rgen Grahn
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

Snapshot::Snapshot(const std::string& time,
		   unsigned depth,
		   const std::unordered_set<std::string>& filter)
    : time{time},
      depth{depth},
      filter{filter}
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

    /**
     * True if the filter is empty, or at least one entry is on the
     * stack.  This is the massf_parse -a option.
     */
    bool matches(const std::unordered_set<std::string>& filter,
		 const std::vector<std::string>& stack)
    {
	if (filter.empty()) return true;
	for (auto& addr : stack) {
	    if (filter.count(addr)) return true;
	}
	return false;
    }

    /**
     * True if a call chain with size 'sz' and the nNN: text 'nn'
     * should be collected and printed.
     */
    bool collectable(const std::string& nn, size_t sz, size_t depth)
    {
	if (depth && sz > depth) return false;
	if (nn=="n0:") return true;
	return sz == depth;
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

    if (collectable(nn, stack.size(), depth) && matches(filter, stack)) {
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
