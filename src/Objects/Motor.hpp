#pragma once
#include "../EmbeddedSimLib/SimObject.h"

class Motor : public SimObject {
    private:
        float speed = 0.0f;
        int portId = -1;
        float torque = 0.18f;
    public:
        Motor() = default;

        void SetConnectedPort(ISimController* pController, int id);

        PortState GetInternalPortState() const;
        void UpdateMotor();
        float GetSpeed() const { return speed; }
};