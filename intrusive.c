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

link* link_get_next(link* l) {
  /* offset from a node pointer to a link structure */
  size_t offset = (size_t) l - ((size_t) l->prev->next & ~1);
  /* link field for the next node */
  size_t offset_of_next = (size_t) l->next & ~1;
  return (link*) (offset_of_next + offset);
}

void link_add_after(list *l, link *link_of_node, void *node, link *previous_link) {
  /* remove_from_list */
  link_get_next(link_of_node)->prev = link_of_node->prev;
  link_of_node->prev->next = link_of_node->next;

  /* add_after */
  link_of_node->prev = previous_link;
  link_of_node->next = previous_link->next;

  link_get_next(previous_link)->prev = link_of_node;
  previous_link->next = node;
}

link* get_link_from_node(list *l, person* node) {
  link* lnk = (link*) ((size_t) node + l->offset);
  return lnk;
}

void list_add(list *l, void* node) {
  link *link = get_link_from_node(l, node);
  link_add_after(l, link, node, &l->link);
}

int main(int argc, char *argv[]) {
  person *p = person_create("Robbie", 190);
  person *p2 = person_create("Trunky", 1);
  size_t offset = offsetof(person, link);

  printf("%s\n", p->name);
  
  list* l = list_create(offset);
  list_add(l, p);
  list_add(l, p2);

  free(p);
  free(p2);
  free(l);
  return 0;
}
