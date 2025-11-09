#include "object.h"

#include "component.h"
#include "components/transform.h"
#include "components/renderer.h"
#include "components/sprite_renderer.h"

std::vector<Object*> ObjectRenderer::objects;

void ObjectRenderer::TryAddObject(Object* object) {
    if (object == nullptr) { return; }
    Renderer* renderer = object->GetComponent<Renderer>();
    if (renderer == nullptr) { return; }
    objects.push_back(object);
}

void ObjectRenderer::RemoveObject(Object* object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

void ObjectRenderer::Render(Graphic& graphic) {
    sort(objects.begin(), objects.end(), [](Object* a, Object* b) {
        SpriteRenderer* a_renderer = a->GetComponent<SpriteRenderer>();
        if (a_renderer == nullptr) { return false; }
        SpriteRenderer* b_renderer = b->GetComponent<SpriteRenderer>();
        if (b_renderer == nullptr) { return false; }

        if (a_renderer->z_index == b_renderer->z_index) {
            return a->transform->GetGlobalY() < b->transform->GetGlobalY();
        }
        return a_renderer->z_index < b_renderer->z_index;
    });

    for (auto& object : objects) {
        object->Render(graphic);
    }
}

Object::Object() {
    transform = new Transform();
    AddComponent(transform);
}

Object::~Object() {
    ObjectRenderer::RemoveObject(this);
    for (auto& comp : components) {
        delete comp;
    }
    components.clear();
    for (auto& child : children) {
        delete child;
    }
    children.clear();
}

void Object::AddChild(Object* child) {
    children.push_back(child);
    child->parent = this;
    ObjectRenderer::TryAddObject(child);
}

void Object::AddComponent(Component* component) {
    components.push_back(component);
    component->object = this;
    ObjectRenderer::TryAddObject(this);
}

void Object::Update(double delta_time) {
    for (auto& component : components)
        component->Update(delta_time);

    for (auto& child : children)
        child->Update(delta_time);
}

void Object::Render(Graphic& graphic) {
    for (auto& component : components)
        component->Render(graphic);
}
