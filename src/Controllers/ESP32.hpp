#pragma once
#include "../SimController.h"

class ESP32 : public SimController<35> {
	public:

		ESP32();
		using SimController<35>::SetPort;
		using SimController<35>::GetPort;
		
		void SetPortMode() override {};
};