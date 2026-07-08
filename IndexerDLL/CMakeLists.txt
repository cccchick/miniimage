add_library(IndexerDLL SHARED
    ./src/indexer.cpp
)

target_compile_definitions(IndexerDLL
    PRIVATE
    INDEXER_EXPORTS
)

target_include_directories(IndexerDLL
    PUBLIC
    include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(IndexerDLL
    PRIVATE
    FeatureExtractorDLL
)
