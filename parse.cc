/* Copyright (c) 2021 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "snapshot.h"
#include "split.h"

#include <iostream>
#include <algorithm>
#include <memory>

namespace {

    /**
     * State machine across the input. The caller is responsible for
     * matching start/end_snapshot, and not starting unless she's going
     * to add().
     */
    struct Machine {
	explicit Machine(std::ostream& os) : os{os} {}
	void start_snapshot(const std::string& time);
	void add(const std::string& s);
	void end_snapshot();

	std::ostream& os;
	std::unique_ptr<Snapshot> now;
    };

    void Machine::start_snapshot(const std::string& time)
    {
	now = std::make_unique<Snapshot>(time);
    }

    void Machine::add(const std::string& s)
    {
	now->add(s);
	os << s << '\n';
    }

    void Machine::end_snapshot()
    {
	now->put(os);
    }

    bool starts_with(const std::string& haystack,
                     const std::string& needle)
    {
        auto i = std::search(begin(haystack), end(haystack),
                             begin(needle), end(needle));
        return i==begin(haystack);
    }

    bool is_time(const std::string& s)
    {
	return starts_with(s, "time=");
    }

    bool is_n(const std::string& s)
    {
	const auto v = split(s, 2);
	const auto& n = v[0];
	if (n.front() != 'n') return false;
	return n.back()==':';
    }
}

int main()
{
    Machine m {std::cout};
    std::string time;
    bool in_snapshot = false;

    std::string s;
    while (std::getline(std::cin, s)) {
	if (is_time(s)) {
	    auto v = split("=", s);
	    time = v[1];
	    if (in_snapshot) {
		m.end_snapshot();
		in_snapshot = false;
	    }
	}
	else if (is_n(s)) {
	    if (time.size()) {
		in_snapshot = true;
		m.start_snapshot(time);
		time = "";
	    }
	    m.add(s);
	}
    }

    if (in_snapshot) {
	m.end_snapshot();
    }
}
