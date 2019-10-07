file(GLOB CURSEDL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(cursedl WIN32 ${CURSEDL_SOURCES})

target_include_directories(cursedl PUBLIC ${FLTK_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/deps/zipper)
target_link_libraries(cursedl ${CMAKE_THREAD_LIBS_INIT} Zipper-static ${LIBZ_LIBRARY} ${EXTRA_LIBS}  cpr ${FLTK_LIBRARIES} nlohmann_json stdc++fs)
set_target_properties(cursedl PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)