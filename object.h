#pragma once

#include <vector>
#include <algorithm>
#include "graphic.h"
#include "sprite.h"

class Object;
class Component;
class Transform;

struct Renderable {
    Object* object;
    bool has_renderer;
};

class ObjectRenderer {
public:
    static std::vector<Object*> objects;
    static std::vector<Renderable*> renderables;
    static void TryAddObject(Object* object);
    static void RemoveObject(Object* object);
    static void Render(Graphic& graphic);
};

class Object {
public:
    Object* parent = nullptr;
    std::vector<Object*> children;
    std::vector<Component*> components;
    Transform* transform = nullptr;
    Object();
    ~Object();
    void AddChild(Object* child);
    void AddComponent(Component* component);
    void Update(double delta_time);
    void Render(Graphic& graphic);
    virtual void OnRender(Graphic& graphic) {}
    
    template<typename T>
    T* GetComponent() {
        for (auto* comp : components) {
            T* result = dynamic_cast<T*>(comp);
            if (result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }
    
    template<typename T>
    std::vector<T*> GetComponents() {
        std::vector<T*> results;
        for (auto* comp : components) {
            T* result = dynamic_cast<T*>(comp);
            if (result != nullptr) {
                results.push_back(result);
            }
        }
        return results;
    }
};
