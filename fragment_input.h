#pragma once

class Object;

struct FragmentInput {
    int world_x, world_y;
    int local_x, local_y;
    Object* object;
    int x, y;
    double u, v;
    void* data;
};