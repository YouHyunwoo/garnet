#include "object.h"

std::vector<Object*> ObjectRenderer::objects;

void ObjectRenderer::AddObject(Object* object) {
    objects.push_back(object);
}

void ObjectRenderer::RemoveObject(Object* object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

void ObjectRenderer::Render(Graphic& graphic) {
    sort(objects.begin(), objects.end(), [](Object* a, Object* b) {
        if (a->z_index == b->z_index) {
            return a->GetGlobalY() < b->GetGlobalY();
        }
        return a->z_index < b->z_index;
    });

    for (auto& object : objects) {
        object->Render(graphic);
    }
}

Object::~Object() {
    ObjectRenderer::RemoveObject(this);
    for (auto& child : children) {
        delete child;
    }
    children.clear();
}

void Object::AddChild(Object* child) {
    children.push_back(child);
    child->parent = this;
    ObjectRenderer::AddObject(child);
}

void Object::Render(Graphic& graphic) {
    double global_x = 0.0, global_y = 0.0;
    GetGlobalOrigin(global_x, global_y);
    graphic.Save();
    graphic.Translate(static_cast<int32_t>(global_x + x), static_cast<int32_t>(global_y + y));
    OnRender(graphic);
    if (sprite != nullptr) {
        graphic.DrawSprite(0, 0, *sprite, this);
    }
    graphic.Restore();
}

void Object::GetGlobalOrigin(double& out_x, double& out_y) {
    out_x = 0;
    out_y = 0;
    Object* current = parent;
    while (current != nullptr) {
        out_x += current->x;
        out_y += current->y;
        current = current->parent;
    }
}

void Object::GetGlobalPosition(double& out_x, double& out_y) {
    GetGlobalOrigin(out_x, out_y);
    out_x += x;
    out_y += y;
}

double Object::GetGlobalX() {
    double out_x = 0;
    Object* current = this;
    while (current != nullptr) {
        out_x += current->x;
        current = current->parent;
    }
    return out_x;
}

double Object::GetGlobalY() {
    double out_y = 0;
    Object* current = this;
    while (current != nullptr) {
        out_y += current->y;
        current = current->parent;
    }
    return out_y;
}