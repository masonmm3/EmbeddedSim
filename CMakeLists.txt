cmake_minimum_required(VERSION 3.24)

project(EmbeddedSim)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_subdirectory(extern/pybind11)

set(SOURCE_FILES 
    main.cpp 
    src/EmbeddedSimLib/SimEvent.h
    src/EmbeddedSimLib/EventDispatcher.h
    src/EmbeddedSimLib/SimController.h 
    src/EmbeddedSimLib/SimObject.h
    src/Controllers/ESP32.hpp
    src/Controllers/ESP32.cpp
     src/Objects/LED.hpp
    src/Objects/LED.cpp
    src/Objects/Switch.hpp
    src/Objects/Switch.cpp
    src/Objects/PWMReader.cpp
    src/Objects/PWMReader.hpp
    src/Objects/Motor.hpp
    src/Objects/Motor.cpp
    src/PythonWrapper.cpp
)

pybind11_add_module(EmbeddedSimLib ${SOURCE_FILES})


target_link_libraries(EmbeddedSimLib PRIVATE pybind11::module)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE pybind11::embed)