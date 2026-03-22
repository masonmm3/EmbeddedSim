#pragma once
#include "../SimObject.h"

class Switch : public SimObject {
    private:
        unsigned closed:1;
        int HighId;
        int LowId;
        bool shortLow = false;
        bool shortHigh = false;
    public:
        void SetConnectedPort(ISimController* pController, int HighId, int LowId);

        PortState GetInternalPortState();

        void SwapSwitch();
        void EnableSwitch();
        void DisableSwitch();

        void UpdateSwitch();
        
};