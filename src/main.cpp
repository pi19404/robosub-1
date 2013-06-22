#include <stdio.h>
#include <string.h>

#include <robosub.h>

int main(int argc, char **argv) {
  if (argc == 2 && (strncmp(argv[1], "-v", 2) == 0 ||
                    strncmp(argv[1], "--version", 9) == 0)) {
    printf("%s Version %d.%d\n",
           argv[0], ROBOSUB_VERSION_MAJOR, ROBOSUB_VERSION_MINOR);
    return 0;
  }
}

