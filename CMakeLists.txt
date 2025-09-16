cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(OpticalSim)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
set(sources ${sources} ${PROJECT_SOURCE_DIR}/sim.cc)

add_executable(sim ${sources})
target_link_libraries(sim ${Geant4_LIBRARIES} ${Geant4_UIVIS_LIBRARIES})

file(GLOB MACRO_FILES "${PROJECT_SOURCE_DIR}/macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR}/macros)

add_custom_target(OpticalSim DEPENDS sim)
