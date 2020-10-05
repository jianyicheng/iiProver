set(CMAKE_BUILD_TYPE Debug)
add_library(boogieGen MODULE boogieGen.cpp)

target_compile_features(boogieGen PRIVATE cxx_range_for cxx_auto_type)
SET(CMAKE_CXX_FLAGS "-fopenmp -fno-rtti -fPIC")
