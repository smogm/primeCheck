INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

ADD_LIBRARY(millerRabin millerRabin.cpp millerrabinParallelBase.cpp millerRabinBase.cpp)
