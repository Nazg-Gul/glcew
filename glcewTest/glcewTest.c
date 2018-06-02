#include <stdlib.h>
#include <stdio.h>
#include "glcew.h"
#include <string.h>

int main(int argc, char* argv[]) {
  (void) argc;  // Ignored.
  (void) argv;  // Ignored.
  if (glcewInit() == GLCEW_SUCCESS) {
    printf("libGL found\n");
  }
  else {
    printf("libGL not found\n");
  }
  return EXIT_SUCCESS;
}
