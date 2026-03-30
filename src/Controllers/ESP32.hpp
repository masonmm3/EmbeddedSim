#pragma once
#include "../EmbeddedSimLib/SimController.h"
#include <map>

struct ESP32PWM {
	std::size_t port;
	float dutyCycle;
	bool lastState;
	long long counterNs = 0; 
    long long periodNs = 200'000'000; // 200ms in nanoseconds
};

class ESP32 : public SimController<35> {
	public:
		ESP32();

		using SimController<35>::SetPort;
		using SimController<35>::GetPort;
		
		void SetPortMode() override;
		void SetPortPWM(std::size_t port, uint8_t dutyCycle);
	private:
		std::map<int, ESP32PWM> pwmHandlers;
		void PWMUpdate();
};

