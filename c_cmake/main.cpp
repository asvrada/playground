#include <iostream>

#include "common/macro.h"
#include "try_linux_api/mmap_related.h"

void hello_world() {
    std::cout << "Hello, World!" << std::endl;
}

int main() {
    hello_world();

    CHECK_RET(try_memfd_create_with_map_shared());

    return 0;
}
