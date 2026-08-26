cmake_minimum_required(VERSION 3.20)

project(FactoryFlowCollector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 외부 라이브러리
find_package(Boost REQUIRED COMPONENTS system)
find_package(PkgConfig REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Ubuntu의 libpqxx-dev는 pkg-config 파일을 제공
pkg_check_modules(PQXX REQUIRED libpqxx)

# Collector 실행 파일
add_executable(FactoryFlowCollector
    FactoryFlow.cpp
    DatabaseManager.cpp
    Equipment.cpp
    FactorySystem.cpp
    Lot.cpp
    LotRelation.cpp
    ProductionEvent.cpp
    TcpServer.cpp
    TelemetryMessage.cpp
    TelemetryParser.cpp
    TelemetryProcessor.cpp
    WorkOrder.cpp
)

# libpqxx 헤더 위치
target_include_directories(FactoryFlowCollector PRIVATE
    ${PQXX_INCLUDE_DIRS}
)

# libpqxx 라이브러리 위치
target_link_directories(FactoryFlowCollector PRIVATE
    ${PQXX_LIBRARY_DIRS}
)

# 라이브러리 연결
target_link_libraries(FactoryFlowCollector PRIVATE
    Boost::system
    ${PQXX_LIBRARIES}
    nlohmann_json::nlohmann_json
)