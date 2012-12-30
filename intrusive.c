#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <stddef.h> /* offsetof */
#include "intrusive.h"

void link_init(link *l, size_t offset) {
  l->next = (void*) ((size_t) l + 1 - offset);
  l->prev = l;
}

list* list_create(size_t offset) {
  list *l = (list*) malloc(sizeof(list));
  l->offset = offset;
  link_init(&l->link, offset);
  return l;
}

static link* list_get_next(link* l) {
  /* offset from a node pointer to a link structure */
  size_t offset = (size_t) l - ((size_t) l->prev->next & ~1);
  /* link field for the next node */
  size_t offset_of_next = (size_t) l->next & ~1;
  return (link*) (offset_of_next + offset);
}

static void list_remove(list *l, link *lnk) {
  list_get_next(lnk)->prev = lnk->prev;
  lnk->prev->next = lnk->next;
}

static void list_add_before(list *l, link *link_of_node, void *node, link *next_link) {
  list_remove(l, link_of_node);

  link_of_node->prev = next_link->prev;
  link_of_node->next = link_of_node->prev->next;

  next_link->prev->next = node;
  next_link->prev = link_of_node;
}

static void list_add_after(list *l, link *link_of_node, void *node, link *previous_link) {
  list_remove(l, link_of_node);

  link_of_node->prev = previous_link;
  link_of_node->next = previous_link->next;

  list_get_next(previous_link)->prev = link_of_node;
  previous_link->next = node;
}

static link* list_get_link_from_node(list *l, void* node) {
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
