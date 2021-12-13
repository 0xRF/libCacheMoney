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

#include <cache.hpp>
#include <iostream>

int main(int argc, char** argv){


   size_t l1 = cache::get_l1_cache_size();

   std::cout << l1 << std::endl;


    return 0;
}