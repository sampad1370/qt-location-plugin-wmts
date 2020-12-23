if (STATIC_PLUGIN)
    set (LIB_TYPE STATIC)
else ()
    set (LIB_TYPE SHARED)
endif ()

add_library(wmtslogic STATIC
    serviceparser.h
    serviceparser.cpp
    thirdparty/pugixml.cpp
)

target_include_directories(wmtslogic
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_include_directories(wmtslogic SYSTEM
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty>
)


add_library(qgeoservices_wmts ${LIB_TYPE}
    qgeomapreplywmts.h
    qgeomapreplywmts.cpp
    qgeoserviceproviderpluginwmts.h
    qgeoserviceproviderpluginwmts.cpp
    qgeotiledmappingmanagerenginewmts.h
    qgeotiledmappingmanagerenginewmts.cpp
    qgeotilefetcherwmts.h
    qgeotilefetcherwmts.cpp
    qgeotileproviderwmts.h
    qgeotileproviderwmts.cpp
)

target_include_directories(qgeoservices_wmts
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_include_directories(qgeoservices_wmts SYSTEM
PRIVATE
    /usr/local/opt/qt/lib/QtPositioning.framework/Versions/5/Headers/5.10.0
    /usr/local/opt/qt/lib/QtLocation.framework/Versions/5/Headers/5.10.0
    /usr/local/opt/qt/lib/QtLocation.framework/Versions/5/Headers
)


if (STATIC_PLUGIN)
    target_compile_definitions(qgeoservices_wmts
    PRIVATE
        QT_STATICPLUGIN
    )
else ()
    if(CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
        message(STATUS "Gen: Multi config")
        set_target_properties (qgeoservices_wmts PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>/geoservices)
    else ()
        message(STATUS "Gen: Single config")
        set_target_properties (qgeoservices_wmts PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/geoservices)
    endif ()
endif()

target_link_libraries(qgeoservices_wmts wmtslogic Qt5::Gui Qt5::Location Qt5::Positioning Qt5::LocationPrivate)
set_target_properties (qgeoservices_wmts PROPERTIES AUTOMOC ON)

add_subdirectory(test)
