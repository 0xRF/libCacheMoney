//
// Created by rf on 12/20/21.
//

#include <attacks/prime_probe.hpp>
#include <functional>
#include <cache.hpp>
#include <l1.hpp>
#include <optional>
#include <utils.hpp>
#include <iostream>

using namespace cache_money;
using namespace cache;

//std::array<std::array<uintptr_t, 8>, 64>
//prime_probe::generate_mapped_addresses (uintptr_t address, uintptr_t true_start, size_t array_size)
//{
//  auto cache_sets = std::array<std::array<uintptr_t, 8>, 64> ();
//
//  for (size_t set = 0; set < l1::set_count (); set++)
//	{
//	  uintptr_t ptr = address * l1::line_size ();
//	  for (size_t block = 0; block < l1::assoc (); block++)
//		{
//		  std::cout << utils::get_address_set (ptr + block * l1::) << std::endl;
//		}
//	}
//}
std::array<std::array<uintptr_t, 8>, 64> prime_probe::generate_mapped_addresses (uintptr_t address, uintptr_t true_start, size_t array_size)
{
  auto cache_sets = std::array<std::array<uintptr_t, 8>, 64> ();

  for (size_t cache_set = 0; cache_set < cache::l1::set_count (); cache_set++)
	{
	  uintptr_t ptr = address;
	  auto tags = std::vector<uint64_t> ();
	  for (size_t block = 0; block < cache::l1::assoc (); block++)
		{
		  auto tag = utils::get_address_tag (ptr);
		  while (utils::get_address_set (ptr) != cache_set ||
				 (std::find (tags.begin (), tags.end (), tag) != tags.end ()))
			{
			  ptr += sizeof (uintptr_t);
			  tag = utils::get_address_tag (ptr);

			  if (ptr > true_start + array_size)
				throw;
			}
		  cache_sets[cache_set][block] = ptr;
		  tags.push_back (tag);
		  ptr += sizeof (uintptr_t);
		}
	}
  return cache_sets;
}
