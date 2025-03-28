add_executable(megafuse main.cpp
    requestlistener.h requestlistener.cpp
    megaservice.h megaservice.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
target_link_libraries(megafuse PRIVATE PkgConfig::deps)

target_link_libraries(megafuse PRIVATE MEGA::SDKlib)
