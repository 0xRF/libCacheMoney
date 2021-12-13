/*
libCacheMoney
Copyright (C) 2021 0xRF

libCacheMoney is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libCacheMoney is program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libCacheMoney.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cache.hpp>
#include <utils.hpp>
#include <intrinsics.hpp>
#include <vector>

namespace cache{

    size_t get_total_cache_size() {
        return 512*1024; //TODO IMPLEMENT
    }

    size_t get_l1_cache_size(){
        uint64_t l1_speed = utils::get_cache_baseline_speed();
        size_t cacheSize = get_total_cache_size();
        char* buffer = new char[cacheSize];
        for(int i = 0; i < cacheSize; i++){
            uint64_t time = intrinsics::memaccesstime((uintptr_t)buffer + i);
            if(time > l1_speed + 40)
                return i;
        }
    }
    size_t get_l2_cache_size(){}
    size_t get_l3_cache_size(){}

}