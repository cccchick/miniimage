add_executable(App
    app.cpp
)

target_link_libraries(App
    PRIVATE
    ImageLoaderDLL
    FeatureExtractorDLL
    IndexerDLL
    SearcherDLL
    Qt6::Widgets
)

# 构建完成后把项目 DLL 和 MinGW 运行时库复制到 App.exe 同级目录，
# 避免运行时加载到系统/Anaconda 里不兼容的 libwinpthread/libstdc++。
get_filename_component(MINGW_BIN ${CMAKE_CXX_COMPILER} DIRECTORY)

add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ImageLoaderDLL>
        $<TARGET_FILE:FeatureExtractorDLL>
        $<TARGET_FILE:IndexerDLL>
        $<TARGET_FILE:SearcherDLL>
        $<TARGET_FILE_DIR:App>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MINGW_BIN}/libwinpthread-1.dll"
        "${MINGW_BIN}/libgcc_s_seh-1.dll"
        "${MINGW_BIN}/libstdc++-6.dll"
        $<TARGET_FILE_DIR:App>
)
