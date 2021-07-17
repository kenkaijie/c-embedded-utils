if (NOT CEMB_CFG_OWN_CMAKE)
    # Overrides for configuring our internal CMOCKA
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
    set(WITH_CMOCKERY_SUPPORT OFF CACHE BOOL "Install a cmockery header" FORCE)
    set(WITH_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
    set(UNIT_TESTING OFF CACHE BOOL "Build with unit testing" FORCE)
    set(PICKY_DEVELOPER ON CACHE BOOL "Build with picky developer flags" FORCE)
    add_subdirectory(cmocka)
endif()