INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/base")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/miller-rabin")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/atkin")

# add here your prime check implementation folder
ADD_SUBDIRECTORY(miller-rabin)
ADD_SUBDIRECTORY(atkin)
ADD_SUBDIRECTORY(base)

ADD_EXECUTABLE (primecheck main.cpp)

# add library to link into exec
TARGET_LINK_LIBRARIES(primecheck miller-rabin atkin basePrime)

# needed for later use?
#SET_TARGET_PROPERTIES(primecheck PROPERTIES LINK_FLAGS "-lpthread")
