#include "ESP32.hpp"
#include <iostream>

ESP32::ESP32() {
    std::cout << "Initializing ESP32 GPIO pins...\n";
    ports.reset();
}