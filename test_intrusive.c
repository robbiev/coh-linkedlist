#include <stdio.h>
#include "minunit.h"

int tests_run = 0;

static char* test_foo() {
  MU_ASSERT("meh", 0 == 0);
  return 0;
}

static char* all_tests() {
  MU_RUN_TEST(test_foo);
  return 0;
}

int main(int argc, char *argv[]) {
  char *result = all_tests();
  if (result) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
