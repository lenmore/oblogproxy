INCLUDE(ExternalProject)

SET(SPDLOG_SOURCES_DIR ${THIRD_PARTY_PATH}/spdlog)
SET(SPDLOG_INSTALL_DIR ${THIRD_PARTY_PATH}/install/spdlog)
SET(SPDLOG_LIBRARIES "${SPDLOG_INSTALL_DIR}/lib/libspdlog.a" CACHE FILEPATH "spdlog library." FORCE)
set(SPDLOG_INCLUDE_DIR "${SPDLOG_INSTALL_DIR}/include")

if (NOT EXISTS ${SPDLOG_INCLUDE_DIR})
    execute_process(COMMAND mkdir -p ${SPDLOG_INCLUDE_DIR} COMMAND_ERROR_IS_FATAL ANY)
endif ()

add_library(spdlog STATIC IMPORTED GLOBAL)

if (NOT WITH_DEPS)
    ExternalProject_Add(
            extern_spdlog
            ${EXTERNAL_PROJECT_LOG_ARGS}
            PREFIX ${SPDLOG_SOURCES_DIR}
            GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
            GIT_TAG v1.11.0
            UPDATE_COMMAND ""
            BUILD_IN_SOURCE 1
            COMMAND $(MAKE) --silent
            INSTALL_COMMAND mkdir -p ${SPDLOG_INSTALL_DIR}/lib COMMAND cp -r include ${SPDLOG_INSTALL_DIR}/ COMMAND cp libspdlog.a ${SPDLOG_LIBRARIES}
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${SPDLOG_INSTALL_DIR}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DSPDLOG_BUILD_EXAMPLE=OFF
            -DSPDLOG_BUILD_EXAMPLE_HO=OFF
            -DSPDLOG_BUILD_ALL=OFF
            -DSPDLOG_PREVENT_CHILD_FD=ON
    )
    add_dependencies(spdlog extern_spdlog)
endif ()

set_target_properties(spdlog PROPERTIES IMPORTED_LOCATION ${SPDLOG_LIBRARIES})
target_include_directories(spdlog INTERFACE ${SPDLOG_INCLUDE_DIR})
