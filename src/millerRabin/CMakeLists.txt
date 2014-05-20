INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

ADD_LIBRARY(millerRabin millerRabin.cpp millerRabinParallelBases.cpp millerRabinParallelPrimes.cpp millerRabinBase.cpp)
