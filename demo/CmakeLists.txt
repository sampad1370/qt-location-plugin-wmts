qt5_add_resources(UI_RESOURCES resources.qrc)
add_executable(wmtsdemo
    demo.qml
    main.cpp
    ${UI_RESOURCES}
)

target_include_directories(wmtsdemo
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(wmtsdemo Qt5::Gui Qt5::QuickControls2 Qt5::Location)
set_target_properties(wmtsdemo PROPERTIES AUTOMOC ON)

if (STATIC_PLUGIN)
    target_link_libraries(wmtsdemo qgeoservices_wmts)
endif ()

if (MSVC)
    set_target_properties(wmtsdemo PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
elseif (MINGW)
    set_target_properties(wmtsdemo PROPERTIES LINK_FLAGS_RELEASE "-Wl,-subsystem,windows")
endif ()
