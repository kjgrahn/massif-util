/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef MASSIF_SNAPSHOT_H
#define MASSIF_SNAPSHOT_H

#include <string>
#include <unordered_set>
#include <vector>
#include <iosfwd>

class Graph;

/**
 * A detailed valgrind --tool=massif snapshot, which is a set of call
 * chains and, for each, the number of bytes allocated that way.
 *
 * It's grown input line by input line.
 */
class Snapshot {
public:
    Snapshot(const std::string& time,
	     unsigned depth = 0,
	     const std::unordered_set<std::string>& filter = {});
    void add(const std::string& s);

    void put(std::ostream& os) const;
    void put(Graph& graph) const;

private:
    const std::string time;
    const unsigned depth;
    const std::unordered_set<std::string> filter;
    std::vector<std::string> stack;

    struct Entry {
	Entry(std::string size, std::string path) : size{size}, path{path} {}
	std::string size;
	std::string path;
    };
    std::vector<Entry> ee;
};

inline
Snapshot& operator<< (Snapshot& ss, const std::string& s)
{
    ss.add(s);
    return ss;
}

#endif
