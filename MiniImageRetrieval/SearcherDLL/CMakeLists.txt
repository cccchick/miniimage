add_library(SearcherDLL SHARED
    src/searcher.cpp
)

target_compile_definitions(SearcherDLL
    PRIVATE
    SEARCHER_EXPORTS
)

target_include_directories(SearcherDLL
    PUBLIC
    include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(SearcherDLL
    PRIVATE
    FeatureExtractorDLL
)
