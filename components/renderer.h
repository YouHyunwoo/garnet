#pragma once

#include <vector>
#include "../component.h"

class Material;

class Renderer : public Component {
public:
    std::vector<Material*> materials;
    ~Renderer();
    virtual void Render(Graphic& graphic) = 0;
};
