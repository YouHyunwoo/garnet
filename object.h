#pragma once

#include <vector>
#include <algorithm>
#include "graphic.h"
#include "sprite.h"

class Object;

class ObjectRenderer {
public:
    static std::vector<Object*> objects;
    static void AddObject(Object* object);
    static void RemoveObject(Object* object);
    static void Render(Graphic& graphic);
};

class Object {
public:
    double x = 0.0, y = 0.0;
    double z_index = 0.0;
    std::vector<Object*> children;
    Object* parent = nullptr;
    Sprite* sprite = nullptr;
    ~Object();
    void AddChild(Object* child);
    void Render(Graphic& graphic);
    virtual void OnRender(Graphic& graphic) {}
    void GetGlobalOrigin(double& out_x, double& out_y);
    void GetGlobalPosition(double& out_x, double& out_y);
    double GetGlobalX();
    double GetGlobalY();
};
