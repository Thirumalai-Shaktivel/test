#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <Kokkos_Core.hpp>
#include <lfortran_intrinsics.h>

template <typename T>
Kokkos::View<T*> from_std_vector(const std::vector<T> &v)
{
    Kokkos::View<T*> r("r", v.size());
    for (size_t i=0; i < v.size(); i++) {
        r(i) = v[i];
    }
    return r;
}

namespace {

void main2() {
        int x;
        x = 2 + 3;
        std::cout << x << std::endl;
}

}
int main(int argc, char* argv[])
{
    Kokkos::initialize(argc, argv);
    main2();
    Kokkos::finalize();
    return 0;
}
