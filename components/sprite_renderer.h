#pragma once

#include "renderer.h"

class Sprite;
class Graphic;

class SpriteRenderer : public Renderer {
public:
    Sprite* sprite = nullptr;
    double z_index = 0.0;
    SpriteRenderer(Sprite* sprite, Material* material=nullptr, double z_index = 0.0);
    void Render(Graphic& graphic) override;
};
