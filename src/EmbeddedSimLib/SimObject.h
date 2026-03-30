#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include <cstddef>
#include <memory>
#include <map>
#include "ISimController.h"


class SimObject {
	protected:
		std::map<std::size_t, ISimController*> ports;
	public:
		virtual ~SimObject() = default;

		void SetConnectedPort(ISimController* pController);

		virtual PortState GetInternalPortState(std::size_t id) { return PortState::Low;};
};

#endif

