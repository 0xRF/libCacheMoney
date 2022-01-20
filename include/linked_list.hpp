//
// Created by rf on 1/17/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_LINKED_LIST_HPP
#define LIBCACHEMONEY_INCLUDE_LINKED_LIST_HPP

#include <cstddef>
#include <cstdint>
#include <intrinsics.hpp>
typedef struct alignas(64) node_t {
  node_t *prev;
  node_t *next;
} node;

//TODO prepare for porting to C
struct linked_list {
  node *start;
  node *head;
  size_t length;
};

inline void list_init(linked_list *list, node *node) {
  list->start = node;
  list->start->prev = nullptr;
  list->start->next = nullptr;
  list->head = list->start;
  list->length++;
}

linked_list list_create(node *start = nullptr) {
  linked_list l;
  if (start!=nullptr)
	list_init(&l, start);
  return l;
}

inline void list_push_back(linked_list *list, node *node) {
  if (list->length==0) [[unlikely]] {
	list_init(list, node);
	return;
  }
  list->head->next = node;
  node->prev = list->head;
  node->next = nullptr;
  list->head = node;
  list->length++;
}

inline void list_push_front(linked_list *list, node *node) {
  if (list->length==0) [[unlikely]] {
	list_init(list, node);
	return;
  }
  list->start->prev= node;
  node->next = list->start;
  node->prev= nullptr;
  list->start = node;
  list->length++;
}


inline node *list_pop_front(linked_list *list) {
  node *ret = list->start;
  list->start = list->start->next;
  list->start->prev = nullptr;
  list->length--;
  return ret;
}

inline node *list_pop_back(linked_list *list) {
  node *ret = list->head;
  list->head = list->head->prev;
  list->head->next = nullptr;
  list->length--;
  return ret;
}

/// not needed
/// \param ptr
/// \return
inline size_t node_tree_length(node *ptr) {
  size_t len = 0;
  while (ptr) {
	len++;
	ptr = ptr->next;
  }
  return len;
}

inline void traverse_list_skylake(node *ptr) {
  while (ptr && ptr->next && ptr->next->next) {
	intrinsics::maccess::normal(ptr);
	intrinsics::maccess::normal(ptr->next);
	intrinsics::maccess::normal(ptr->next->next);
	intrinsics::maccess::normal(ptr);
	intrinsics::maccess::normal(ptr->next);
	intrinsics::maccess::normal(ptr->next->next);
	ptr = ptr->next;
  }
}

inline
void
// Traverse list, interleave with access to first element
// To keep it in private caches
traverse_zigzag_victim(node *ptr, uintptr_t victim) {
  while (ptr) {
	intrinsics::maccess::normal(ptr);
	intrinsics::maccess::normal(victim);
	ptr = ptr->next;
  }
}

#endif // LIBCACHEMONEY_INCLUDE_LINKED_LIST_HPP
