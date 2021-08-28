/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef MASSIF_SNAPSHOT_H
#define MASSIF_SNAPSHOT_H

#include <string>
#include <vector>

/**
 * A detailed valgrind --tool=massif snapshot, which is a set of call
 * chains and, for each, the number of bytes allocated that way.
 *
 * It's grown input line by input line.
 */
class Snapshot {
public:
    explicit Snapshot(const std::string& time);
    void add(const std::string& s);

private:
    const std::string time;
    std::vector<std::string> stack;

    struct Line {
	unsigned level;
	unsigned size;
	std::string addr;
    };
};

inline
Snapshot& operator<< (Snapshot& ss, const std::string& s)
{
    ss.add(s);
    return ss;
}

#endif
