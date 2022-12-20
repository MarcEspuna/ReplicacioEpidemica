cmake_minimum_required(VERSION 3.0.0)
project(ReplicacioEpidemica VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(dependencies/spdlog)
add_subdirectory(dependencies/efsw)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

if (NOT TARGET efsw)
    find_package(efsw REQUIRED)
endif()

add_executable(ReplicacioEpidemica src/main.cpp src/sockets/Server.cpp src/sockets/Client.cpp src/sockets/Socket.cpp src/nodes/Node.cpp src/nodes/CoreNode.cpp src/nodes/LayerOneNode.cpp src/nodes/LayerTwoNode.cpp src/Log.cpp src/transactions/FileWatcher.cpp src/mutex/LamportMutex.cpp src/nodes/MsgHandler.cpp src/transactions/Transaction.cpp src/transactions/TransactionReader.cpp src/Controller.cpp)
add_dependencies(ReplicacioEpidemica spdlog efsw)

set(LIBS Ws2_32.lib spdlog::spdlog efsw)

target_compile_features(ReplicacioEpidemica PRIVATE cxx_std_17)
target_include_directories(ReplicacioEpidemica PRIVATE dependencies/spdlog/include dependencies/spdlog/include/spdlog dependencies/efsw/include/efsw dependencies/efsw/include src)
target_precompile_headers(ReplicacioEpidemica PRIVATE src/Commons.h)
target_link_libraries(ReplicacioEpidemica PUBLIC ${LIBS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
