#pragma once
#include "../EmbeddedSimLib/SimObject.h"

class PWMReader : public SimObject {
    private:
        float currentCycle = 0;
        long long onDuration = 0;
        long long offDuration = 0;
        bool lastState = false;
        int portId = -1;
        ISimController* controller = nullptr;

        void calculateDutyCycle();

    public:
        void SetConnectedPort(ISimController* pController, int id);
        PortState GetInternalPortState() const;    
        void UpdatePWM();  
        float GetPWMCycle();  
};