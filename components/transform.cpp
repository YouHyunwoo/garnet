#include "transform.h"

#include "../object.h"

void Transform::GetGlobalPosition(Vector3& out_position) {
    if (object->parent == nullptr) {
        out_position = position;
        return;
    }

    Transform* parent_transform = object->parent->GetComponent<Transform>();
    if (parent_transform == nullptr) {
        out_position = position;
        return;
    }

    Vector3 parent_position;
    parent_transform->GetGlobalPosition(parent_position);
    out_position.x = parent_position.x + position.x;
    out_position.y = parent_position.y + position.y;
    out_position.z = parent_position.z + position.z;
}

double Transform::GetGlobalX() {
    if (object->parent == nullptr) {
        return position.x;
    }

    Transform* parent_transform = object->parent->GetComponent<Transform>();
    if (parent_transform == nullptr) {
        return position.x;
    }

    return parent_transform->GetGlobalX() + position.x;
}

double Transform::GetGlobalY() {
    if (object->parent == nullptr) {
        return position.y;
    }

    Transform* parent_transform = object->parent->GetComponent<Transform>();
    if (parent_transform == nullptr) {
        return position.y;
    }

    return parent_transform->GetGlobalY() + position.y;
}