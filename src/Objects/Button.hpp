#pragma once
#include "../SimObject.h"

class Button : public SimObject {
    private:
        unsigned pushed:1;
        int HighId;
        int LowId;
        bool HighSide = false;
        bool LowSide = false;
        bool shortLow = false;
        bool shortHigh = false;
    public:
        
        void SetConnectedPort(ISimController* pController, int HighId, int LowId);

        PortState GetInternalPortState();

        void PushButton();

        void UpdateButton();
};