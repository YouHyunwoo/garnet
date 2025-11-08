#pragma once

#include "color.h"
#include "fragment_input.h"

class Shader {
public:
    void* data;
    virtual Color Shade(const FragmentInput &input) = 0;
};