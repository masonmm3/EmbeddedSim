cmake_minimum_required(VERSION 3.24)

# Fixed typo from 'Embeded' to 'Embedded' if desired
project(EmbeddedSim)

# Remove the comma after SimController.cpp
set(SOURCE_FILES 
    main.cpp 
    src/SimController.h 
    src/SimObject.h
    src/Controllers/ESP32.hpp
    src/Controllers/ESP32.cpp
    src/Objects/LED.hpp
    src/Objects/LED.cpp
    src/Objects/Switch.hpp
    src/Objects/Switch.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})