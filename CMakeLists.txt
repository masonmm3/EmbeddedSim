cmake_minimum_required(VERSION 3.24)

# Fixed typo from 'Embeded' to 'Embedded' if desired
project(EmbeddedSim)

# Remove the comma after SimController.cpp
set(SOURCE_FILES 
    main.cpp 
    src/SimController.h 
    src/SimObject.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})