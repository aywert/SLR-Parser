cmake_minimum_required(VERSION 3.10)  
project(SLR)

set(CMAKE_CXX_STANDARD 17)

find_package(FLEX REQUIRED)

FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/include/gram.l ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp)

add_executable(SLR src/main.cpp ${FLEX_Lexer_OUTPUTS} src/SLR_gen.cpp)


target_include_directories(SLR PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SLR PRIVATE ${FLEX_LIBRARIES})
#cmake -S ./ -B build
#cmake --build build