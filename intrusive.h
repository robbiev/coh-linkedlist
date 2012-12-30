typedef struct link {
  struct link *prev; 
  void *next;
} link;

typedef struct list {
  struct link link;
  size_t offset;
} list;

void link_init(link *l, size_t offset);
void* link_prev(link *lnk); 
void* link_next(link *lnk);

list* list_create(size_t offset);
void list_insert_head(list *l, void* node);
void list_insert_tail(list *l, void* node);
void* list_head(list *l);
void* list_tail(list *l);
