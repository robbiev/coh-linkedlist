#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <stddef.h> /* offsetof */

typedef struct link {
  struct link *prev;
  void *next;
} link;

typedef struct {
  link link;
  size_t offset;
} list;

typedef struct {
  int weight;
  char *name;
  link link;
} person;

void link_init(link *l, size_t offset) {
  l->next = (void*) ((size_t) l + 1 - offset);
  l->prev = l;
}

person* person_create(char *name, int weight) {
  person *p = (person*)malloc(sizeof(person));
  p->name = name;
  p->weight = weight;
  link_init(&p->link, offsetof(person, link));
  return p;
}

list* list_create(size_t offset) {
  list *l = (list*) malloc(sizeof(list));
  l->offset = offset;
  link_init(&l->link, offset);
  return l;
}

// todo link next/prev, list head/tail

link* list_get_next(link* l) {
  /* offset from a node pointer to a link structure */
  size_t offset = (size_t) l - ((size_t) l->prev->next & ~1);
  /* link field for the next node */
  size_t offset_of_next = (size_t) l->next & ~1;
  return (link*) (offset_of_next + offset);
}

void list_remove(list *l, link *lnk) {
  list_get_next(lnk)->prev = lnk->prev;
  lnk->prev->next = lnk->next;
}

void list_add_before(list *l, link *link_of_node, void *node, link *next_link) {
  /* remove_from_list */
  list_remove(l, link_of_node);

  /* add_before */
  link_of_node->prev = next_link->prev;
  link_of_node->next = link_of_node->prev->next;

  next_link->prev->next = node;
  next_link->prev = link_of_node;
}

void list_add_after(list *l, link *link_of_node, void *node, link *previous_link) {
  /* remove_from_list */
  list_remove(l, link_of_node);

  /* add_after */
  link_of_node->prev = previous_link;
  link_of_node->next = previous_link->next;

  list_get_next(previous_link)->prev = link_of_node;
  previous_link->next = node;
}

link* list_get_link_from_node(list *l, person* node) {
  link* lnk = (link*) ((size_t) node + l->offset);
  return lnk;
}

void list_insert_head(list *l, void* node) {
  link *link = list_get_link_from_node(l, node);
  list_add_after(l, link, node, &l->link);
}

void list_insert_tail(list *l, void* node) {
  link *link = list_get_link_from_node(l, node);
  list_add_before(l, link, node, &l->link);
}

void* link_prev(link *lnk) {
  void* node = lnk->prev->prev->next;
  if ((size_t) node & 1)
    return NULL;
  return node;
}

void* link_next(link *lnk) {
  if ((size_t) lnk->next & 1)
    return NULL;
  return lnk->next;
}

void* list_head(list *l) {
  return link_next(&l->link); 
}

void* list_tail(list *l) {
  return link_prev(&l->link);
}

void test_insert_head() {
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
}

void test_insert_tail() {
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
}

int main(int argc, char *argv[]) {
  test_insert_head();
  test_insert_tail();
  return 0;
}
