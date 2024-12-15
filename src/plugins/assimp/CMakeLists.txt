# SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>
# SPDX-License-Identifier: BSD-2-Clause

find_package(assimp REQUIRED)

add_konvex_plugin(assimpplugin assimpplugin.cpp)
target_link_libraries(assimpplugin PRIVATE assimp::assimp)
