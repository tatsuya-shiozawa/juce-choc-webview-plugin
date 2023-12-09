include(CPM)

CPMAddPackage("gh:catchorg/Catch2#v3.4.0")
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)