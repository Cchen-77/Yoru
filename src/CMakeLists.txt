add_subdirectory(ext)

set(YORU_ALL_SOURCES
    yoru/base/Vector.cpp
    yoru/base/Transform.cpp
    yoru/base/Ray.cpp
    yoru/base/BoundingBox.cpp
    yoru/color/Spectrum.cpp
    yoru/camera/Camera.cpp
    yoru/film/Film.cpp
    yoru/main.cpp
)
set(YORU_ALL_INCLUDES
    yoru/base/Vector.h
    yoru/base/Transform.h
    yoru/base/BoundingBox.h
    yoru/base/Ray.h
    yoru/color/Spectrum.h
    yoru/camera/Camera.h
    yoru/film/Film.h
)

add_executable(Yoru ${YORU_ALL_SOURCES} ${YORU_ALL_INCLUDES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/yoru/ FILES ${YORU_ALL_SOURCES} ${YORU_ALL_INCLUDES})
target_link_libraries(Yoru PRIVATE Yoru-ext)
target_include_directories(Yoru PRIVATE yoru)
