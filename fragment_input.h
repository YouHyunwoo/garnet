#pragma once

class Object;
class Material;

struct FragmentInput {
    int screen_x, screen_y;
    int local_x, local_y;
    Object* object;
    Material* material;
    int x, y;
    double u, v;
    void* data;
};