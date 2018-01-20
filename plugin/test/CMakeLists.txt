add_executable(wmtsplugintest
    serviceparsertest.cpp
    data/servicedesc.xml
)

target_link_libraries(wmtsplugintest wmtslogic)
target_compile_definitions(wmtsplugintest PRIVATE -DTEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
