#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include <bitset>
#include <iostream>
#include <bitset>
#include <cstddef>
#include <map>
#include "ISimController.h"

class SimObject {
	protected:
		std::map<int, ISimController*> ports;
	public:
		virtual ~SimObject() = default;

		void SetConnectedPort(ISimController* pController);

		virtual PortState GetInternalPortState(int id) = 0;
};

#endif

