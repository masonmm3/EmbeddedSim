#pragma once
#include "../EmbeddedSimLib/SimObject.h"

class Switch : public SimObject {
    private:
        bool closed = false;
        int HighId = -1;
        int LowId = -1;
        bool isUpdating = false;
        bool shortLow = false;
        bool shortHigh = false;
    public:
        void SetConnectedPort(ISimController* pController, int HighId, int LowId);
        PortState GetInternalPortState() const;

        void SwapSwitch();
        void EnableSwitch();
        void DisableSwitch();
        void UpdateSwitch();
};