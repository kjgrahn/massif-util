/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef MASSIF_GRAPH_H
#define MASSIF_GRAPH_H

#include <string>
#include <vector>
#include <array>
#include <set>
#include <iostream>

/**
 * Rendering a dot(1) digraph.
 */
class Graph {
public:
    Graph(std::ostream& os, const std::string& name);
    ~Graph();
    void add(const std::vector<std::string>& chain);

private:
    std::ostream& os;
    using Pair = std::array<std::string, 2>;
    std::set<Pair> seen;
};

#endif
