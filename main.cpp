#include "src/ISimController.h"
#include "src/controllers/Esp32.hpp"
#include "src/Objects/LED.hpp"
#include "src/Objects/Button.hpp"


#include <thread>
#include <chrono>
#include <cstdio>


int main(void) {

	ESP32 esp;

	LED led1;

	led1.SetConnectedPort((ISimController*)&esp, 10);

	Button button1;

	button1.SetConnectedPort((ISimController*)&esp, 1, 2);

	bool state = 0;

	esp.SetPort(1, 1);

	while (true) {

		//Testing section
		printf("LED1 state: %s\n", led1.GetInternalPortState() == PortState::High ? "ON" : "OFF");
		printf("Button state: %s\n", button1.GetInternalPortState(true) == PortState::High ? "Pushed" : "Not Pushed");
		printf("Port 2 State: %s\n", esp.GetPort(2) == PortState::High ? "HIGH" : "LOW");
		//Testing section end

		state = !state;

		esp.SetPort(10, state);
		
		button1.PushButton();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}