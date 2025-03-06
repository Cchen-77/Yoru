add_subdirectory(json)

add_library(Yoru-ext INTERFACE)
target_include_directories(Yoru-ext INTERFACE json/include eigen)
target_link_libraries(Yoru-ext INTERFACE nlohmann_json)