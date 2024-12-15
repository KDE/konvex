# SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>
# SPDX-License-Identifier: BSD-2-Clause

add_library(konvex-interface SHARED)
target_sources(konvex-interface PRIVATE
        konvexplugininterface.h
        konvexplugininterface.cpp)
target_link_libraries(konvex-interface PUBLIC
        KF6::CoreAddons
        Qt6::Quick3D)

generate_export_header(konvex-interface EXPORT_FILE_NAME konvex_export.h BASE_NAME KonvexInterface)
