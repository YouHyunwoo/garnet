#pragma once

#include "../component.h"
#include "../vector.h"

class Transform : public Component {
public:
    Vector3 position;
    void GetGlobalPosition(Vector3& out_position);
    double GetGlobalX();
    double GetGlobalY();
};
