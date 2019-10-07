# common "library" sources
file(GLOB CURSEDL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

# drivers
file(GLOB CURSEDL_UI_DRIVER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/drivers/ui/*.cc)
file(GLOB CURSEDL_CLI_DRIVER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/drivers/cli/*.cc)

# ui driver
add_executable(cursedl WIN32 ${CURSEDL_SOURCES} ${CURSEDL_UI_DRIVER_SOURCES})
target_include_directories(cursedl PUBLIC ${FLTK_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/deps/zipper)
target_link_libraries(cursedl ${CMAKE_THREAD_LIBS_INIT} Zipper-static ${LIBZ_LIBRARY} cpr ${FLTK_LIBRARIES} nlohmann_json stdc++fs)
set_target_properties(cursedl PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# CLI driver
add_executable(cursedl-cli ${CURSEDL_SOURCES} ${CURSEDL_CLI_DRIVER_SOURCES})
target_include_directories(cursedl-cli PUBLIC ${PROJECT_SOURCE_DIR}/deps/zipper)
target_link_libraries(cursedl-cli ${CMAKE_THREAD_LIBS_INIT} Zipper-static ${LIBZ_LIBRARY} cpr nlohmann_json stdc++fs)
set_target_properties(cursedl-cli PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)