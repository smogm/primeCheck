INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/base")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/millerRabin")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/atkin")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/eratosthenes")

# add here your prime check implementation folder
ADD_SUBDIRECTORY(millerRabin)
ADD_SUBDIRECTORY(atkin)
ADD_SUBDIRECTORY(eratosthenes)
ADD_SUBDIRECTORY(base)

ADD_EXECUTABLE (primecheck main.cpp)

# add libraries to exec
TARGET_LINK_LIBRARIES(primecheck millerRabin atkin atkinthreaded atkinthreadedvar2 atkinthreadedatomic eratosthenes basePrime)

# needed for std::thread
SET_TARGET_PROPERTIES(primecheck PROPERTIES LINK_FLAGS "-lpthread")
