// This was to test that the xv6 works

#include "types.h"
#include "user.h"

int main (int argc, char *argv[]) {
    printf(1, "hello my name is '%s'\n", argv[0]);
    exit();
}