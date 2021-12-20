/*
libcachemoney
copyright (c) 2021 0xrf

libcachemoney is free software: you can redistribute it and/or modify
        it under the terms of the gnu general public license as published by
the free software foundation, either version 3 of the license, or
(at your option) any later version.

libcachemoney is program is distributed in the hope that it will be useful,
        but without any warranty; without even the implied warranty of
merchantability or fitness for a particular purpose.  see the
gnu general public license for more details.

you should have received a copy of the gnu general public license
along with libcachemoney.  if not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include <intrinsics.hpp>
#include <l1.hpp>
#include <random>
#include <utils.hpp>
#include <attacks/prime_probe.hpp>
#include <bitset>
#include <iostream>
#include <vector>
#include <iomanip>
#include <utils.hpp>

using namespace cache;
using namespace cache_money;

constexpr size_t get_array_size ()
{ return 3 * 1024 * 1024; } // 1 MiB

uintptr_t get_buffer_start_aligned_to_page (uintptr_t buffer)
{
  for (uintptr_t address = buffer; address < buffer + get_array_size ();
	   address++)
	{
	  if (utils::is_page_start (address))
		return address;
	}
  return 0;
}

/// Populate RCX with zeroes
inline void memsett (uintptr_t buffer)
{ // ensure works on windows and linux
  register size_t RCX asm("rcx") = l1::size ();
  register uintptr_t RDI asm("rdi") = buffer;
  asm("xor %rax, %rax");
  asm("rep stosb");
}

int main ()
{
  auto true_buffer_start = (uintptr_t)malloc (1024 * 1024);
  auto address = utils::get_aligned_address (true_buffer_start, get_array_size());
  if (get_array_size () - (true_buffer_start - address) < l1::size ())
	{
	  std::cout << "Failed to properly map address" << std::endl;
	  return EXIT_FAILURE;
	}

  auto cache_mapping = prime_probe::generate_mapped_addresses (address, true_buffer_start, get_array_size ());
  size_t RCX = 0;
  size_t RBX = 0;
  size_t ASSOC = l1::assoc ();
  size_t SET_COUNT = l1::set_count ();
  size_t ITER = 0;

  auto baseline = l1::speed ();

  const size_t SAMPLES = 10000;
  auto timing =
	  std::array<std::array<std::array<uint32_t, 8>, 64>, SAMPLES> ();

  memsett (address);

  for (ITER = 0; ITER < SAMPLES; ITER++)
	{
	  for (RCX = 0; RCX < SET_COUNT; RCX++)
		{
		  for (RBX = 0; RBX < ASSOC; RBX++)
			{
			  utils::prefetch_range (cache_mapping[RCX][RBX], 1);
			  //measure the time it takes to accsess the set
			  *(uintptr_t *)cache_mapping[RCX][RBX] = -1;
			  timing[ITER][RCX][RBX] = intrinsics::memaccesstime (cache_mapping[RCX][RBX]);
			}
		}
	}

  for (RCX = 0; RCX < SET_COUNT; RCX++)
	{
	  for (RBX = 0; RBX < ASSOC; RBX++)
		{
		  uint64_t avg = 0;
		  for (ITER = 0; ITER < SAMPLES; ITER++)
			{
			  avg += timing[ITER][RCX][RBX];
			}
		  avg = avg / SAMPLES;
		  std::cout << avg << " ";
		}
	  std::cout << std::endl;
	}
//loop through all cache sets, clear and measure time to read the addresses

  return 0;
}