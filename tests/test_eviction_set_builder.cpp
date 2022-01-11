//
// Created by rf on 1/11/22.
//

#include <eviction_set_builder.hpp>
#include <utils.hpp>
#include <l1.hpp>
#include <iostream>
#include <cstring>

//
//std::vector<uint64_t> bad_find_eviction_set_l1(uint32_t set) {
//
//  std::vector<uintptr_t> evictionSet(cache::l1::assoc());
//
//  const uint32_t bufferSize = 1024*1024;
//  auto buffer = (uintptr_t)malloc(bufferSize);
//  auto address = utils::get_aligned_address((uintptr_t)buffer, bufferSize);
//
//  uintptr_t ptr = address;
//  auto tags = std::vector<uint64_t>();
//  for (size_t block = 0; block < cache::l1::assoc(); block++) {
//	auto tag = utils::get_address_tag(ptr);
//	while (utils::get_address_set(ptr)!=set ||
//		(find(tags.begin(), tags.end(), tag)!=tags.end())) {
//	  ptr += sizeof(uintptr_t);
//	  tag = utils::get_address_tag(ptr);
////		if (ptr > (uintptr_t) buffer + bufferSize)
////		  throw std::runtime_error("Out of bounds");
//	}
//	evictionSet[block] = ptr;
//	tags.push_back(tag);
//	ptr += sizeof(uintptr_t);
//  }
//
//  return evictionSet;
//}
//

typedef elem<eviction_set_element> evict_element;
evict_element *find_eviction_set_l1(uint32_t set) {

  std::vector<uintptr_t> evictionSet(cache::l1::assoc());

  const uint32_t bufferSize = 1024*1024;
  auto buffer = (evict_element *)malloc(bufferSize);
  auto address = utils::get_aligned_address((uintptr_t)buffer, bufferSize);

  uintptr_t ptr = address;
  auto tags = std::vector<uint64_t>();
  for (size_t block = 0; block < cache::l1::assoc(); block++) {
	auto tag = utils::get_address_tag(ptr);
	while (utils::get_address_set(ptr)!=set ||
		(find(tags.begin(), tags.end(), tag)!=tags.end())) {
	  ptr += sizeof(uintptr_t);
	  tag = utils::get_address_tag(ptr);
//		if (ptr > (uintptr_t) buffer + bufferSize)
//		  throw std::runtime_error("Out of bounds");
	}
	evictionSet[block] = ptr;
	tags.push_back(tag);
	ptr += sizeof(uintptr_t);
  }

  auto iter = evictionSet.begin();
  evict_element *ll = (evict_element *)(*evictionSet.begin());

  while (ll) {
	if (iter==evictionSet.begin())
	  ll->prev = NULL;
	else
	  ll->prev = (evict_element *)*std::prev(iter);

	iter = std::next(iter);
	ll->next = (evict_element *)*iter;

	ll = ll->next;
  }

  return (evict_element *)(*evictionSet.begin());
}
//  if (std::count(m_mapped.begin(), m_mapped.end(), 0) != 0)
//	throw std::runtime_error("Could find all the required addresses");

evict_element *find_fake_eviction_set_l1(uint32_t set) {

  std::vector<uintptr_t> evictionSet(cache::l1::assoc());

  const uint32_t bufferSize = 1024*1024;
  auto buffer = (evict_element *)malloc(bufferSize);
  memset(buffer, 0, bufferSize);
  auto address = utils::get_aligned_address((uintptr_t)buffer, bufferSize);

  uintptr_t ptr = address;
  auto tags = std::vector<uint64_t>();
  for (size_t block = 0; block < cache::l1::assoc(); block++) {
	auto tag = utils::get_address_tag(ptr);
	while (utils::get_address_set(ptr)!=set ||
		(find(tags.begin(), tags.end(), tag)!=tags.end())) {
	  ptr += sizeof(uintptr_t);
	  tag = utils::get_address_tag(ptr);
//		if (ptr > (uintptr_t) buffer + bufferSize)
//		  throw std::runtime_error("Out of bounds");
	}
	evictionSet[block] = ptr;
	tags.push_back(tag);
	ptr += sizeof(uintptr_t);
  }

  //Set the second element of the eviction set the one which has a non unique tag (same as first tag), but a unique address
  // Need to make sure this is spaced far enough apart form the previous, so lazily allocate more memory
  buffer = (evict_element *)malloc(bufferSize);
  memset(buffer, 0, bufferSize);
  address = utils::get_aligned_address((uintptr_t)buffer, bufferSize);
  ptr = address + 0x100;
  auto tag = utils::get_address_tag(ptr);
  while (utils::get_address_set(ptr)!=set && tag!=*tags.begin()) {
	ptr += sizeof(uintptr_t);
	tag = utils::get_address_tag(ptr);
  }

  evictionSet[1] = ptr;
  auto iter = evictionSet.begin();
  evict_element *ll = (evict_element *)(*evictionSet.begin());

  while (ll) {
	if (iter==evictionSet.begin())
	  ll->prev = NULL;
	else
	  ll->prev = (evict_element *)*std::prev(iter);

	iter = std::next(iter);
	ll->next = (evict_element *)*iter;

	ll = ll->next;
  }

  return (evict_element *)(*evictionSet.begin());
}

int main() {

  eviction_set_builder builder;

  auto S = find_eviction_set_l1(23);
  //
  bool isEviction = builder.test(S, 0, cache::l1::speed() + 10);
  std::cout << isEviction << std::endl;

  auto S2 = find_fake_eviction_set_l1(24);
  isEviction = builder.test(S2, 0, cache::l1::speed());
  std::cout << isEviction << std::endl;

  return 0;
}