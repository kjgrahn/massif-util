#include <iostream>
#include <vector>

#include <unistd.h>

template <class T>
struct Vector {
    void add();
    std::vector<T> v;
};

template <class T>
void Vector<T>::add()
{
    v.push_back({});
    v.push_back({});
    v.push_back({});
}

void wait()
{
    usleep(0.001e6);
    std::cerr.put('.');
}

int main()
{
    Vector<char> a;
    Vector<int> b;
    for (unsigned i=0; i<20000; i++) {
	a.add();
	b.add();
	wait();
    }
    std::cerr.put('\n');
}
