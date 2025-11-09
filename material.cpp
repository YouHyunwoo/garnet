#include "material.h"

using namespace std;

Material::Material(Shader* shader, unordered_map<string, void*> properties_map, unordered_map<string, void*> references_map)
    : shader(shader), properties(properties_map), references(references_map) {}

Material::~Material() {
    for (auto& pair : properties) {
        delete pair.second;
    }
    properties.clear();
    references.clear();
}

void Material::SetProperty(const string& name, void* value) {
    properties[name] = value;
}

void* Material::GetProperty(const string& name) {
    if (properties.find(name) != properties.end()) {
        return properties[name];
    }
    return nullptr;
}

void Material::SetReference(const string& name, void* value) {
    references[name] = value;
}

void* Material::GetReference(const string& name) {
    if (references.find(name) != references.end()) {
        return references[name];
    }
    return nullptr;
}
