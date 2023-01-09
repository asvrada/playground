#include <stdio.h>

#include "common/macro.h"
#include "try_linux_api/mmap_related.h"

int main() {
    CHECK_RET(try_memfd_create_with_map_shared());

    return 0;
}
