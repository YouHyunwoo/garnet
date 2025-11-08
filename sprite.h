#pragma once

class Texture;
class Shader;
class Graphic;

struct SpriteInfo {
    double pivot_x = 0.0, pivot_y = 0.0;
};

class Sprite {
public:
    double pivot_x = 0.0, pivot_y = 0.0;
    Texture* texture = nullptr;
    Shader* shader = nullptr;
    Sprite(Texture* texture);
    void Render(int x, int y, Graphic& graphic);
};
