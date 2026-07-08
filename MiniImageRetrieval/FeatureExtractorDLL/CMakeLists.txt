add_library(FeatureExtractorDLL SHARED
    ./src/feature_extract.cpp
)

target_compile_definitions(FeatureExtractorDLL
    PRIVATE
    FEATURE_EXPORTS
)

target_include_directories(FeatureExtractorDLL
    PUBLIC
    include
    ${CMAKE_SOURCE_DIR}
    PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(FeatureExtractorDLL
    PRIVATE
    ${OpenCV_LIBS}
)