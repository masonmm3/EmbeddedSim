#include <cstdio>
#include "src/ISimController.h"
#include "src/controllers/Esp32.hpp"
#include "src/Objects/LED.hpp"
#include <thread>
#include <chrono>


int main(void) {

	ESP32 esp;

	LED led1;

	led1.SetConnectedPort((ISimController*)&esp, 10);

	bool state = 0;

	while (true) {
		
		//Testing section
		printf("LED1 state: %s\n", led1.GetInternalPortState() == PortState::High ? "ON" : "OFF");
		//Testing section end

		state = !state;

		esp.SetPort(10, state);
		
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}