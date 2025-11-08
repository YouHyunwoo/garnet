#include "asset.h"

#include "texture.h"
#include "sprite.h"
#include "shader.h"
#include "log.h"

std::unordered_map<std::string, Texture*> Asset::textures;
std::unordered_map<std::string, Sprite*> Asset::sprites;
std::unordered_map<std::string, Shader*> Asset::shaders;

void Asset::Initialize(
    std::unordered_map<std::string, std::string>& texture_info,
    std::unordered_map<std::string, SpriteInfo>& sprite_info,
    std::unordered_map<std::string, Shader*>& shader_info
) {
    for (auto& pair : texture_info) {
        const std::string& name = pair.first;
        const std::string& path = pair.second;
        textures[name] = Texture::Load(path);
        if (textures[name] == nullptr) {
            Log::Error("Failed to load texture: " + path);
        }
    }

    for (auto& pair : sprite_info) {
        const std::string& name = pair.first;
        Texture* texture = textures[name];
        if (texture == nullptr) {
            continue;
        }
        sprites[name] = new Sprite(texture);
        sprites[name]->pivot_x = pair.second.pivot_x;
        sprites[name]->pivot_y = pair.second.pivot_y;
    }

    for (auto& pair : shader_info) {
        const std::string& name = pair.first;
        shaders[name] = pair.second;
        sprites[name]->shader = shaders[name];
    }
}

void Asset::Clear() {
    for (auto& pair : textures) {
        delete pair.second;
    }
    textures.clear();

    for (auto& pair : sprites) {
        delete pair.second;
    }
    sprites.clear();

    for (auto& pair : shaders) {
        delete pair.second;
    }
    shaders.clear();
}

Texture* Asset::GetTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

Sprite* Asset::GetSprite(const std::string& name) {
    auto it = sprites.find(name);
    if (it != sprites.end()) {
        return it->second;
    }
    return nullptr;
}

Shader* Asset::GetShader(const std::string& name) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        return it->second;
    }
    return nullptr;
}
