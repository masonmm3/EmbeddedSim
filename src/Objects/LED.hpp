#pragma once
#include "../SimObject.h"

class LED : public SimObject {
    private:
        unsigned enabled:1;
        int portId = -1;
    public:
        LED() = default;

        void SetConnectedPort(ISimController* pController, int id);

        PortState GetInternalPortState() const;
        void UpdateLED();
};