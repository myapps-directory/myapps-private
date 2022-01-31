
add_library(myapps_private_utility statistic.hpp src/statistic.cpp)

target_link_libraries(myapps_private_utility OpenSSL::Crypto SolidFrame::solid_system)

add_subdirectory(test)

