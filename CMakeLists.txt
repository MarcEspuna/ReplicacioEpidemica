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

add_executable(ReplicacioEpidemica main.cpp Server.cpp Client.cpp Socket.cpp Node.cpp CoreNode.cpp LayerOneNode.cpp LayerTwoNode.cpp Log.cpp FileWatcher.cpp LamportMutex.cpp MsgHandler.cpp Transaction.cpp TransactionReader.cpp)

set(LIBS Ws2_32.lib spdlog::spdlog efsw)

target_compile_features(ReplicacioEpidemica PRIVATE cxx_std_17)
target_include_directories(ReplicacioEpidemica PRIVATE dependencies/spdlog/include dependencies/spdlog/include/spdlog dependencies/efsw/include/efsw dependencies/efsw/include)
target_precompile_headers(ReplicacioEpidemica PRIVATE Commons.h)
target_link_libraries(ReplicacioEpidemica PUBLIC ${LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
