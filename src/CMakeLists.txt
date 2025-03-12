add_subdirectory(ext)

set(YORU_ALL_SOURCES
    yoru/as/AS.cpp
    yoru/as/EmbreeAS.cpp
    yoru/as/NaiveAS.cpp

    yoru/base/Common.cpp
    yoru/base/Vector.cpp
    yoru/base/Transform.cpp
    yoru/base/Ray.cpp
    yoru/base/BoundingBox.cpp
    yoru/base/Frame.cpp
    yoru/base/Intersection.cpp

    yoru/color/Spectrum.cpp

    yoru/camera/Camera.cpp

    yoru/film/Film.cpp

    yoru/filter/BoxFilter.cpp
    yoru/filter/GaussianFilter.cpp
    yoru/filter/TriangleFilter.cpp
    yoru/filter/MitchellFilter.cpp
    yoru/filter/Filter.cpp

    yoru/integrator/Integrator.cpp
    yoru/integrator/NormalIntegrator.cpp
    yoru/integrator/PTIntegrator.cpp

    yoru/light/Light.cpp
    yoru/light/PointLight.cpp
    yoru/light/DistanceLight.cpp
    yoru/light/AreaLight.cpp

    yoru/texture/Textures.cpp
    yoru/texture/ImageTextures.cpp

    yoru/utils/BufferCache.cpp
    yoru/utils/Sampling.cpp
    yoru/utils/Parallelism.cpp

    yoru/material/Material.cpp
    yoru/material/DiffuseMaterial.cpp
    yoru/material/BxDF/BxDF.cpp
    yoru/material/BxDF/LambertianBRDF.cpp

    yoru/primitive/Primitive.cpp
    yoru/primitive/GeometricPrimitive.cpp
    yoru/primitive/TransformedPrimitive.cpp

    yoru/sampler/IndependentSampler.cpp
    yoru/sampler/StratifiedSampler.cpp
    yoru/sampler/Sampler.cpp

    yoru/shape/Shape.cpp
    yoru/shape/Sphere.cpp
    yoru/shape/Cylinder.cpp
    yoru/shape/Disk.cpp
    yoru/shape/TriangleMesh.cpp

    yoru/main.cpp
)
set(YORU_ALL_INCLUDES
    yoru/as/AS.h
    yoru/as/EmbreeAS.h
    yoru/as/NaiveAS.h

    yoru/base/Common.h
    yoru/base/Vector.h
    yoru/base/Transform.h
    yoru/base/BoundingBox.h
    yoru/base/Ray.h
    yoru/base/Frame.h
    yoru/base/Intersection.h

    yoru/color/Spectrum.h

    yoru/camera/Camera.h

    yoru/film/Film.h

    yoru/filter/BoxFilter.h
    yoru/filter/GaussianFilter.h
    yoru/filter/TriangleFilter.h
    yoru/filter/MitchellFilter.h
    yoru/filter/Filter.h

    yoru/integrator/Integrator.h
    yoru/integrator/NormalIntegrator.h
    yoru/integrator/PTIntegrator.h

    yoru/light/Light.h
    yoru/light/PointLight.h
    yoru/light/DistanceLight.h
    yoru/light/AreaLight.h

    yoru/texture/Textures.h
    yoru/texture/ImageTextures.h

    yoru/utils/BufferCache.h
    yoru/utils/Hash.h
    yoru/utils/Util.h
    yoru/utils/RNG.h
    yoru/utils/Sampling.h
    yoru/utils/Parallelism.h

    yoru/material/Material.h
    yoru/material/DiffuseMaterial.h
    yoru/material/BxDF/BxDF.h
    yoru/material/BxDF/LambertianBRDF.h

    yoru/primitive/Primitive.h
    yoru/primitive/GeometricPrimitive.h
    yoru/primitive/TransformedPrimitive.h

    yoru/sampler/IndependentSampler.h
    yoru/sampler/StratifiedSampler.h
    yoru/sampler/Sampler.h
    
    yoru/shape/Shape.h
    yoru/shape/Sphere.h
    yoru/shape/Cylinder.h
    yoru/shape/Disk.h
    yoru/shape/TriangleMesh.h
)

add_executable(Yoru ${YORU_ALL_SOURCES} ${YORU_ALL_INCLUDES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/yoru/ FILES ${YORU_ALL_SOURCES} ${YORU_ALL_INCLUDES})
target_link_libraries(Yoru PRIVATE Yoru-ext)

find_package(embree 4 REQUIRED)
find_package(TBB REQUIRED)
target_link_libraries(Yoru PRIVATE embree TBB::tbb)
target_include_directories(Yoru PRIVATE yoru)

file(GLOB embree_dlls ${EMBREE_ROOT_DIR}/bin/*.dll)
add_custom_command(TARGET Yoru POST_BUILD        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        ${embree_dlls}    
        $<TARGET_FILE_DIR:Yoru>)
