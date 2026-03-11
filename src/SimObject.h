#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include <bitset>
#include <iostream>
#include <bitset>
#include <cstddef>
#include "SimController.h"



template <size_t N>
class SimObject {
	protected:
		*SimController controller;
	public:
		virtual ~SimController() = default;

		virtual SetConnectedPort(*SimController Controller, std::initializer_list<int> ids);

		virtual internalPortState(int id);	
}

#endif

