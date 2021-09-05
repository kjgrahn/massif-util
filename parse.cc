/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "snapshot.h"
#include "graph.h"
#include "files...h"
#include "rjust.h"
#include "split.h"

#include <iostream>
#include <algorithm>
#include <memory>
#include <unordered_set>

#include <getopt.h>

namespace {

    /**
     * State machine across the input. The caller is responsible for
     * matching start/end_snapshot, and not starting unless she's going
     * to add().
     */
    template <class T>
    struct Machine {
	explicit Machine(T& os) : os{os} {}
	void start_snapshot(const std::string& time,
			    const std::unordered_set<std::string>& filter);
	void add(const std::string& s);
	void end_snapshot();

	T& os;
	std::unique_ptr<Snapshot> now;
    };

    template <class T>
    void Machine<T>::start_snapshot(const std::string& time,
				    const std::unordered_set<std::string>& filter)
    {
	now = std::make_unique<Snapshot>(time, filter);
    }

    template <class T>
    void Machine<T>::add(const std::string& s)
    {
	now->add(s);
    }

    template <class T>
    void Machine<T>::end_snapshot()
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

    void detailed(std::ostream& os, Files& in,
		  const unsigned depth,
		  const std::unordered_set<std::string>& filter)
    {
	Machine<std::ostream> m {os};
	std::string time;
	bool in_snapshot = false;

	std::string s;
	while (in.getline(s)) {
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
		    m.start_snapshot(time, filter);
		    time = "";
		}
		m.add(s);
	    }
	}

	if (in_snapshot) {
	    m.end_snapshot();
	}
    }

    void graph(std::ostream& os, Files& in)
    {
	Graph graph{os, "foo"};
	Machine<Graph> m {graph};
	std::string time;
	bool in_snapshot = false;

	std::string s;
	while (in.getline(s)) {
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
		    m.start_snapshot(time, {});
		    time = "";
		}
		m.add(s);
	    }
	}

	if (in_snapshot) {
	    m.end_snapshot();
	}
    }

    void shallow(std::ostream& os, Files& in)
    {
	std::string time;
	std::string heap;
	std::string heap_extra;

	std::string s;
	while (in.getline(s)) {
	    const auto v = split("=", s);
	    if (v.size() != 2) continue;
	    auto key = v[0];
	    auto val = v[1];
	    if (key == "time") time = val;
	    else if (key == "mem_heap_B") heap = val;
	    else if (key == "mem_heap_extra_B") {
		heap_extra = val;
		os << rjust(12, time) << ' '
		   << rjust(9, heap) << ' '
		   << rjust(9, heap_extra) << '\n';
	    }
	}
    }
}

int main(int argc, char ** argv)
{
    const std::string prog = argv[0] ? argv[0] : "allergyd";
    const std::string usage = "usage: "
	+ prog +
	" [-d depth]"
	" [-a address] ..."
	" file ...\n"
	"       " + prog +
	" --shallow"
	" file ...\n"
	"       " + prog +
	" --graph"
	" file ...\n"
	"       " + prog +
	" --help\n"
	"       " + prog +
	" --version";
    const char optstring[] = "d:a:";
    struct option long_options[] = {
	{"shallow",      0, 0, 'S'},
	{"graph",        0, 0, 'G'},
	{"version", 	 0, 0, 'v'},
	{"help",    	 0, 0, 'h'},
	{0, 0, 0, 0}
    };

    char mode = 'D';
    unsigned depth = 0;
    std::unordered_set<std::string> filter;

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring, &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'S':
	case 'G':
	    mode = ch;
	    break;
	case 'd':
	    depth = std::stoul(optarg);
	    break;
	case 'a':
	    filter.emplace(optarg);
	    break;
	case 'h':
	    std::cout << usage << '\n';
	    return 0;
	case 'v':
	    std::cout << "massif_parse " << "XXX" << '\n'
		      << "Copyright (c) 2021 J Grahn\n";
	    return 0;
	    break;
	case ':':
	case '?':
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	default:
	    break;
	}
    }

    Files in(argv+optind, argv+argc);

    if (mode=='D') {
	detailed(std::cout, in, depth, filter);
    }
    else if (mode=='S') {
	shallow(std::cout, in);
    }
    else if (mode=='G') {
	graph(std::cout, in);
    }

    return 0;
}
