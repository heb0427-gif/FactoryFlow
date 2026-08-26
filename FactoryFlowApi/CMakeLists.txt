cmake_minimum_required(VERSION 3.20)

project(FactoryFlowApi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(PkgConfig REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

pkg_check_modules(PQXX REQUIRED libpqxx)

add_executable(FactoryFlowApi
    FactoryFlowApi.cpp
    DatabaseManager.cpp
)

target_include_directories(FactoryFlowApi PRIVATE
    ${PQXX_INCLUDE_DIRS}
)

target_link_directories(FactoryFlowApi PRIVATE
    ${PQXX_LIBRARY_DIRS}
)

target_link_libraries(FactoryFlowApi PRIVATE
    Boost::system
    ${PQXX_LIBRARIES}
    nlohmann_json::nlohmann_json
)