#including CPM.cmake, a package manager:
#https://github.com/TheLartians/CPM.cmake
include(CPM)

#Fetching fmt from git
#IF you want to instead point it to a local version, you can invoke CMake with
CPMAddPackage("gh:fmtlib/fmt#master")