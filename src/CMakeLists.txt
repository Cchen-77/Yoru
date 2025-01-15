add_subdirectory(ext)

set(YORU_ALL_SOURCES
    yoru/base/Vector.cpp
    yoru/base/Transform.cpp
    yoru/base/Ray.cpp
    yoru/main.cpp
)
set(YORU_ALL_INCLUDES
    yoru/base/Vector.h
    yoru/base/Transform.h
    yoru/base/Ray.h
)

add_executable(Yoru ${YORU_ALL_SOURCES} ${YORU_ALL_INCLUDES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/yoru/ FILES ${YORU_ALL_SOURCES} ${YORU_ALL_INCLUDES})
target_link_libraries(Yoru PRIVATE Yoru-ext)
