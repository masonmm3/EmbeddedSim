#pragma once
#include "../SimObject.h"

class LED : public SimObject {
    private:
        unsigned enabled:1;
        int id;
    public:
        
        void SetConnectedPort(ISimController* pController, int id);

        PortState GetInternalPortState(int id) override;

        void UpdateLED();
};