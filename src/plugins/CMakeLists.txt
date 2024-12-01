function(add_konvex_plugin name)
    kcoreaddons_add_plugin(${name} SOURCES ${ARGN} INSTALL_NAMESPACE "kf6/konvex")
    target_link_libraries(${name} PRIVATE Qt6::Core konvex-interface)
    set_target_properties(${name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf6/konvex")
endfunction()

add_subdirectory(assimp)
