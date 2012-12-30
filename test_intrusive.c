#include "minunit.h"
#include "intrusive.h"
#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <stddef.h> /* offsetof */

int tests_run = 0;

typedef struct {
  int weight;
  char *name;
  link link;
} person;

person* person_create(char *name, int weight) {
  person *p = (person*)malloc(sizeof(person));
  p->name = name;
  p->weight = weight;
  link_init(&p->link, offsetof(person, link));
  return p;
}

char* test_insert_head() {
  person *p = person_create("Robbie", 190);
  person *p2 = person_create("Trunky", 1);
  size_t offset = offsetof(person, link);
  
  list* l = list_create(offset);
  list_insert_head(l, p);
  list_insert_head(l, p2);

  person *phead = (person*) list_head(l);
  person *next = (person*) link_next(&phead->link);

  printf("head insert, head #1: %s\n", phead->name);
  printf("head insert, head #2: %s\n", next->name);

  free(p);
  free(p2);
  free(l);

  return 0;
}

char* test_insert_tail() {
  person *p = person_create("Robbie", 190);
  person *p2 = person_create("Trunky", 1);
  size_t offset = offsetof(person, link);
  
  list* l = list_create(offset);
  list_insert_tail(l, p);
  list_insert_tail(l, p2);

  person *phead = (person*) list_head(l);
  person *next = (person*) link_next(&phead->link);

  printf("tail insert, head #1: %s\n", phead->name);
  printf("tail insert, head #2: %s\n", next->name);

  person *ptail = (person*) list_tail(l);
  person *nextt = (person*) link_next(&ptail->link);

  printf("tail insert, tail #1: %s\n", ptail->name);
  printf("tail insert, tail #2 NULL?: %d\n", nextt == NULL);

  person *prev = (person*) link_prev(&ptail->link);
  person *prev2 = (person*) link_prev(&prev->link);

  printf("tail insert, tail #-1 NULL?: %d\n", prev == NULL);
  printf("tail insert, tail #-2 NULL?: %d\n", prev2 == NULL);

  free(p);
  free(p2);
  free(l);

  return 0;
}

static char* test_foo() {
  MU_ASSERT("meh", 0 == 0);
  return 0;
}

static char* all_tests() {
  MU_RUN_TEST(test_foo);
  MU_RUN_TEST(test_insert_head);
  MU_RUN_TEST(test_insert_tail);
  MU_RUN_TEST(test_foo);
  return 0;
}

int main(int argc, char *argv[]) {
  char *result = all_tests();
  if (result) {
    printf("%s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
