/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "graph.h"

namespace {

    template <class Set, class T>
    bool insert(Set& set, const T& val)
    {
	auto res = set.insert(val);
	return res.second;
    }
}

Graph::Graph(std::ostream& os, const std::string& name)
    : os{os}
{
    os << "digraph " << name << " {\n"
       << "  node [shape=box];\n"
       << "\n";
}

Graph::~Graph()
{
    os << "}\n";
}

/**
 * Add {foo, bar, baz} as
 * "bar" -> "foo";
 * "baz" -> "bar";
 */
void Graph::add(const std::vector<std::string>& chain)
{
    std::string prev;
    for (const auto& s : chain) {
	if (prev.empty()) {
	    prev = s;
	    continue;
	}

	const Pair ab = {s, prev};
	prev = s;
	if (insert(seen, ab)) {
	    os << "  \"" << ab[0] << "\" -> \"" << ab[1] << "\";\n";
	}
    }
}
