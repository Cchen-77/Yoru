add_subdirectory(json)

add_library(Yoru-ext STATIC ext.cpp)
target_include_directories(Yoru-ext PUBLIC json/include eigen stb tinyobjloader)
target_link_libraries(Yoru-ext PUBLIC nlohmann_json)
