set(LULESH_SOURCES
    lulesh.cc
    lulesh-comm.cc
    lulesh-viz.cc
    lulesh-util.cc
    lulesh-init.cc)

set(lulesh_deps variorum)

if(OPENMP_FOUND)
   set(lulesh_openmp_flags "-DLULESH_USE_OPENMP")
   list(APPEND lulesh_deps openmp)
else()
   set(lulesh_openmp_flags "")
endif()

#if (ENABLE_SERIAL)
#    blt_add_executable(
#        NAME        lulesh_ser
#        SOURCES     ${LULESH_SOURCES}
#        DEPENDS_ON  ${lulesh_deps}
#        OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
#
#
#    blt_add_target_compile_flags(TO lulesh_ser FLAGS "-DUSE_MPI=0 ${lulesh_openmp_flags}")
#
#    # install target for lulesh serial
#    install(TARGETS lulesh_ser
#            EXPORT  variorum
#            LIBRARY DESTINATION examples/proxies/lulesh
#            ARCHIVE DESTINATION examples/proxies/lulesh
#            RUNTIME DESTINATION examples/proxies/lulesh
#    )
#endif()

#install(FILES ASCENT_README.md
#        DESTINATION examples/proxies/lulesh)

if(MPI_FOUND)
    set(my_compiler_flags -DUSE_MPI=1 ${lulesh_openmp_flags})

    set(lulesh_par_deps variorum)
    if(OPENMP_FOUND)
        list(APPEND lulesh_par_deps openmp)
    endif()

    add_executable(lulesh_par ${LULESH_SOURCES})
    target_include_directories(lulesh_par PUBLIC ${MPI_CXX_INCLUDE_PATH})
    target_compile_options(lulesh_par PUBLIC ${my_compiler_flags})
    target_link_libraries(lulesh_par ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS} ${lulesh_par_deps})

#    install(TARGETS lulesh_par
#            EXPORT  ascent
#            LIBRARY DESTINATION examples/ascent/proxies/lulesh
#            ARCHIVE DESTINATION examples/ascent/proxies/lulesh
#            RUNTIME DESTINATION examples/ascent/proxies/lulesh
#    )
endif()
