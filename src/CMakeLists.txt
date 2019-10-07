

file(GLOB CURSEDL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

if(WIN32)
    add_executable(cursedl WIN32 ${CURSEDL_SOURCES})
else()
    add_executable(cursedl WIN32 ${CURSEDL_SOURCES})
endif()

target_include_directories(cursedl PUBLIC ${FLTK_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/deps/zipper)
target_link_libraries(cursedl ${CMAKE_THREAD_LIBS_INIT} Zipper-static ${LIBZ_LIBRARY} ${EXTRA_LIBS}  cpr ${FLTK_LIBRARIES} nlohmann_json )
set_target_properties(cursedl PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)