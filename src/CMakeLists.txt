# SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
# SPDX-License-Identifier: BSD-2-Clause

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        controller.h
        controller.cpp
)

if(ANDROID)
    add_library(konvex SHARED
        ${PROJECT_SOURCES}
    )
else()
    add_executable(konvex
      ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(konvex PRIVATE
    Qt::Core
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt::3DCore
    Qt::3DRender
    Qt::3DInput
    Qt::3DLogic
    Qt::3DExtras
    Qt::3DAnimation
    KF6::I18n
    KF6::WindowSystem
    KF6::CoreAddons)
target_compile_definitions(konvex PRIVATE HAVE_WINDOWSYSTEM)

set_target_properties(konvex PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    )

install(TARGETS konvex
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(konvex)
    qt_finalize_executable(konvex)
endif()
