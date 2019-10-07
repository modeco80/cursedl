# CPR
if(WIN32)
	option(CMAKE_USE_OPENSSL "Use OpenSSL code. Experimental" OFF)
endif()

set(BUILD_CPR_TESTS OFF CACHE BOOL "Don't build cpr tests")
find_package(CURL)
if(CURL_FOUND)
	set(USE_SYSTEM_CURL ON CACHE BOOL "Use the system curl for faster builds")
endif()
add_subdirectory(cpr)

# nlohmann json
# We don't need tests, install.
# or single includes.
set(JSON_BuildTests OFF)
set(JSON_Install OFF)
set(JSON_MultipleHeaders ON)
# This will also make anything using ctest not build tests
set(BUILD_TESTING OFF)
add_subdirectory(json)

# Zipper (zip library used to decompress the modpack ZIP)
set(BUILD_TEST OFF CACHE BOOL "no building tests" FORCE)
add_subdirectory(zipper)
