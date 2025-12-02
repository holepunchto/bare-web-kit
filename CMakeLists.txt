cmake_minimum_required(VERSION 4.0)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)

project(bare_web_kit C OBJC)

add_bare_module(bare_web_kit)

target_sources(
  ${bare_web_kit}
  PRIVATE
    binding.m
)

target_link_libraries(
  ${bare_web_kit}
  PUBLIC
    "-framework WebKit"
)
