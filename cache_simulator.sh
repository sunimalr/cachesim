#!/bin/bash

g++ $1 cache.cpp cache.hpp cache_l1d.cpp cache_l1i.cpp infinity_cache.cpp cache_l2.cpp cachesim.cpp lru.cpp victim_cache.cpp prefetcher.cpp -o cachesim

./cachesim $2