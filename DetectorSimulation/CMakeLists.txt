#----------------------------------------------------------------------------
# Setup the project
#
set(CMAKE_CXX_STANDARD 23)

cmake_minimum_required(VERSION 3.16...3.27)
project(DetectorSimulation)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# See the documentation for a guide on how to enable/disable specific components
#
find_package(Geant4 REQUIRED ui_all vis_all)

# Find HepMC3
find_package(HepMC3 REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, use our local headers, and link it to the Geant4 libraries
#
add_executable(DetectorSimulation DetectorSimulation.cc ${sources} ${headers})
target_include_directories(DetectorSimulation PRIVATE include ${HEPMC3_INCLUDE_DIR})
target_link_libraries(DetectorSimulation PRIVATE ${Geant4_LIBRARIES} HepMC3::HepMC3)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(DetectorSimulation_SCRIPTS
    DetectorSimulation.out
    DetectorSimulation.in
    gui.mac
    run1.mac
    run2.mac
    init_vis.mac
    vis.mac
    )

foreach(_script ${DetectorSimulation_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
    )
endforeach()
