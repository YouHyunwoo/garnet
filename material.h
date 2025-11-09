#pragma once

#include <unordered_map>
#include <string>

class Shader;

class Material {
public:
    Shader* shader = nullptr;
    std::unordered_map<std::string, void*> properties;
    std::unordered_map<std::string, void*> references;
    Material(Shader* shader, std::unordered_map<std::string, void*> properties_map={}, std::unordered_map<std::string, void*> references_map={});
    ~Material();
    void SetProperty(const std::string& name, void* value);
    void* GetProperty(const std::string& name);
    void SetReference(const std::string& name, void* value);
    void* GetReference(const std::string& name);
};
