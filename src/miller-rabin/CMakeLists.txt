INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

ADD_LIBRARY(miller-rabin millerRabin.cpp millerrabinParallelBase.cpp millerRabinBase.cpp)
