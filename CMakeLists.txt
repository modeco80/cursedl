cmake_minimum_required(VERSION 3.1)

project(CurseDL CXX)

# add third party depends tree
add_subdirectory(deps)

find_package(FLTK REQUIRED)
find_package(Threads)


add_subdirectory(src)
