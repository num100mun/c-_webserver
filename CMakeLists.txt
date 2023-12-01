cmake_minimum_required(VERSION 3.22)
project(c___webserver2_0)

set(CMAKE_CXX_STANDARD 17)

add_executable(C__main main.cpp)
add_executable(C__EasyTcpServer EasyTcpServer.cpp )
add_executable(C__EasyTcpClient EasyTcpClient.cpp )
add_executable(C__Test testserver.cpp)

#target_link_libraries(C__main PRIVATE ws2_32)
#target_link_libraries(C__EasyTcpServer PRIVATE ws2_32)
#target_link_libraries(C__EasyTcpClient PRIVATE ws2_32)
#target_link_libraries(C__Test PRIVATE ws2_32)