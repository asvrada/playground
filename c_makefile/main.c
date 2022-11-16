
#include <stdio.h>
#include <stdlib.h>

void try_bit_manipulation()
{
    uint8_t h = 0xff;

    uint8_t a = h << 8;
    uint16_t b = h << 8;
    uint16_t c = (h << 8) | h;
    printf("a: %x\nb: %x\nc: %x\n", a, b, c);
}

int main() {
    try_bit_manipulation();
    return 0;
}
