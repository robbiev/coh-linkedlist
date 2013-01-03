#include "intrusive.h"
#include <stddef.h> /* size_t */ 
#include <stdlib.h>

static link* link_get_next(link* l);
static void link_remove(link *lnk);
static void list_add_before(list *l, link *link_of_node, void *node, link *next_link);
static void list_add_after(list *l, link *link_of_node, void *node, link *previous_link);
static link* list_get_link_from_node(list *l, void* node);

void link_init(link *lnk, size_t offset) {
  lnk->next = (void*) ((size_t) lnk + 1 - offset);
  lnk->prev = lnk;
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

int link_is_linked(link *lnk) {
  return lnk->prev != lnk;
}

void link_unlink(link *lnk) {
  link_remove(lnk);

  // end of the list with no offset
  lnk->next = (void*) ((size_t) lnk + 1);
  lnk->prev = lnk;
}

list* list_create(size_t offset) {
  list *l = malloc(sizeof(list));
  if (l) {
    l->offset = offset;
    link_init(&l->link, offset);
  }
  return l;
}

void list_insert_head(list *l, void* node) {
  link *link = list_get_link_from_node(l, node);
  list_add_after(l, link, node, &l->link);
}

void list_insert_tail(list *l, void* node) {
  link *link = list_get_link_from_node(l, node);
  list_add_before(l, link, node, &l->link);
}

void* list_head(list *l) {
  return link_next(&l->link); 
}

void* list_tail(list *l) {
  return link_prev(&l->link);
}

static link* link_get_next(link* lnk) {
  /* offset from a node pointer to a link structure */
  size_t offset = (size_t) lnk - ((size_t) lnk->prev->next & ~1);
  /* link field for the next node */
  size_t offset_of_next = (size_t) lnk->next & ~1;
  return (link*) (offset_of_next + offset);
}

static void link_remove(link *lnk) {
  link_get_next(lnk)->prev = lnk->prev;
  lnk->prev->next = lnk->next;
}

static void list_add_before(list *l, link *link_of_node, void *node, link *next_link) {
  link_remove(link_of_node);

  link_of_node->prev = next_link->prev;
  link_of_node->next = link_of_node->prev->next;

  next_link->prev->next = node;
  next_link->prev = link_of_node;
}

static void list_add_after(list *l, link *link_of_node, void *node, link *previous_link) {
  link_remove(link_of_node);

  link_of_node->prev = previous_link;
  link_of_node->next = previous_link->next;

  link_get_next(previous_link)->prev = link_of_node;
  previous_link->next = node;
}

static link* list_get_link_from_node(list *l, void* node) {
  return (link*) ((size_t) node + l->offset);
}
