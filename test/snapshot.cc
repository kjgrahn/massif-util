#include <snapshot.h>

#include <orchis.h>

namespace snap {

    using orchis::TC;

    void empty(TC)
    {
	Snapshot s("700");
    }

    void one(TC)
    {
	Snapshot s("700");
	s << "n1: 72707 (heap allocation functions) malloc/new/new[], --alloc-fns, etc."
	  << " n1: 72704 0x48F0545: ???"
	  << "  n1: 72704 0x400FFE1: call_init.part.0"
	  << "   n1: 72704 0x40100E8: call_init"
	  << "    n1: 72704 0x40100E8: _dl_init"
	  << "     n0: 72704 0x40010C9: ???";
    }

    void two(TC)
    {
	Snapshot s("700");
	s << "n1: 72707 (heap allocation functions) malloc/new/new[], --alloc-fns, etc."
	  << " n1: 72704 0x48F0545: ???"
	  << "  n1: 72704 0x400FFE1: call_init.part.0"
	  << "   n1: 72704 0x40100E8: call_init"
	  << "    n1: 72704 0x40100E8: _dl_init"
	  << "     n0: 72704 0x40010C9: ???"
	  << "n1: 72707 (heap allocation functions) malloc/new/new[], --alloc-fns, etc."
	  << " n1: 72704 0x58F0545: ???"
	  << "  n1: 72704 0x500FFE1: call_init.part.0"
	  << "   n1: 72704 0x50100E8: call_init"
	  << "    n1: 72704 0x50100E8: _dl_init"
	  << "     n0: 72704 0x50010C9: ???";
    }

    void tree(TC)
    {
	Snapshot s("3762");
	s << "n3: 187392 (heap allocation functions) malloc/new/new[], --alloc-fns, etc."
	  << " n1: 98304 0x10AA97: __gnu_cxx::new_allocator<int>::allocate(unsigned long, void"
	  << "  n1: 98304 0x10A80C: std::allocator_traits<std::allocator<int> >::allocate(std:"
	  << "   n1: 98304 0x10A593: std::_Vector_base<int, std::allocator<int> >::_M_allocate"
	  << "    n1: 98304 0x109F15: void std::vector<int, std::allocator<int> >::_M_realloc_"
	  << "     n1: 98304 0x109A6D: void std::vector<int, std::allocator<int> >::emplace_ba"
	  << "      n2: 98304 0x10986D: std::vector<int, std::allocator<int> >::push_back(int&"
	  << "       n1: 65536 0x1096A9: Vector<int>::add()"
	  << "        n0: 65536 0x10927F: main"
	  << "       n1: 32768 0x10968F: Vector<int>::add()"
	  << "        n0: 32768 0x10927F: main"
	  << " n1: 72704 0x48F0545: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)"
	  << "  n1: 72704 0x400FFE1: call_init.part.0"
	  << "   n1: 72704 0x40100E8: call_init"
	  << "    n1: 72704 0x40100E8: _dl_init"
	  << "     n0: 72704 0x40010C9: ???"
	  << " n1: 16384 0x10A947: __gnu_cxx::new_allocator<char>::allocate(unsigned long, voi"
	  << "  n1: 16384 0x10A705: std::allocator_traits<std::allocator<char> >::allocate(std"
	  << "   n1: 16384 0x10A31D: std::_Vector_base<char, std::allocator<char> >::_M_alloca"
	  << "    n1: 16384 0x109BE7: void std::vector<char, std::allocator<char> >::_M_reallo"
	  << "     n1: 16384 0x1099C5: void std::vector<char, std::allocator<char> >::emplace_"
	  << "      n1: 16384 0x10983B: std::vector<char, std::allocator<char> >::push_back(ch"
	  << "       n1: 16384 0x10964C: Vector<char>::add()"
	  << "        n0: 16384 0x109273: main";
    }

    void chaff(TC)
    {
	Snapshot s("3762");
	s << "n3: 187392 (heap allocation functions) malloc/new/new[], --alloc-fns, etc."
	  << " n1: 98304 0x10AA97: __gnu_cxx::new_allocator<int>::allocate(unsigned long, void"
	  << "  n1: 98304 0x10A80C: std::allocator_traits<std::allocator<int> >::allocate(std:"
	  << "   n1: 98304 0x10A593: std::_Vector_base<int, std::allocator<int> >::_M_allocate"
	  << "    n1: 98304 0x109F15: void std::vector<int, std::allocator<int> >::_M_realloc_"
	  << "     n1: 98304 0x109A6D: void std::vector<int, std::allocator<int> >::emplace_ba"
	  << "      n3: 98304 0x10986D: std::vector<int, std::allocator<int> >::push_back(int&"
	  << "       n1: 65536 0x1096A9: Vector<int>::add()"
	  << "        n0: 65536 0x10927F: main"
	  << "       n1: 32768 0x10968F: Vector<int>::add()"
	  << "        n0: 32768 0x10927F: main"
	  << "       n0: 0 in 1 place, below massif's threshold (1.00%)"
	  << " n1: 72704 0x48F0545: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)"
	  << "  n1: 72704 0x400FFE1: call_init.part.0"
	  << "   n1: 72704 0x40100E8: call_init"
	  << "    n1: 72704 0x40100E8: _dl_init"
	  << "     n0: 72704 0x40010C9: ???"
	  << " n1: 16384 0x10A947: __gnu_cxx::new_allocator<char>::allocate(unsigned long, voi"
	  << "  n1: 16384 0x10A705: std::allocator_traits<std::allocator<char> >::allocate(std"
	  << "   n1: 16384 0x10A31D: std::_Vector_base<char, std::allocator<char> >::_M_alloca"
	  << "    n1: 16384 0x109BE7: void std::vector<char, std::allocator<char> >::_M_reallo"
	  << "     n1: 16384 0x1099C5: void std::vector<char, std::allocator<char> >::emplace_"
	  << "      n2: 16384 0x10983B: std::vector<char, std::allocator<char> >::push_back(ch"
	  << "       n1: 16384 0x10964C: Vector<char>::add()"
	  << "        n0: 16384 0x109273: main"
	  << "       n0: 0 in 2 places, all below massif's threshold (1.00%)";
    }
}
