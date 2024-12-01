find_package(assimp REQUIRED)

add_konvex_plugin(assimpplugin assimpplugin.cpp)
target_link_libraries(assimpplugin PRIVATE assimp::assimp)
