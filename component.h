#pragma once

class Object;
class Graphic;

class Component {
public:
    Object* object = nullptr;
    virtual ~Component();
    virtual void Update(double delta_time) {}
    virtual void Render(Graphic& graphic) {}
};