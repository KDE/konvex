# SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
# SPDX-License-Identifier: BSD-2-Clause

add_compile_options(-fPIC)

add_executable(konvex)

ecm_add_qml_module(konvex
        URI org.kde.konvex
        VERSION 1.0)

ecm_target_qml_sources(konvex SOURCES
        ui/Main.qml
        ui/SettingsPage.qml)

target_sources(konvex PRIVATE
        controller.cpp
        controller.h
        main.cpp)

target_link_libraries(konvex PRIVATE
    Qt::Core
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt6::Quick3D
    KF6::I18n
    KF6::I18nQml
    KF6::WindowSystem
    KF6::CoreAddons
    konvex-interface)
target_compile_definitions(konvex PRIVATE HAVE_WINDOWSYSTEM)

install(TARGETS konvex ${KF6_INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(core)
add_subdirectory(plugins)
