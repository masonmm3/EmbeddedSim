#pragma once
#include "../SimController.h"

class ESP32 : public SimController<35> {
public:

	using SimController<35>::SetPort;
    using SimController<35>::GetPort;
	
	void Init() override;
    void DeInit() override;
    void SetPortMode() override {};
};