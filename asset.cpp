#include "asset.h"

#include "log.h"
#include "texture.h"
#include "sprite.h"
#include "shader.h"
#include "material.h"

using namespace std;

unordered_map<string, Texture*> Asset::Textures;
unordered_map<string, Sprite*> Asset::Sprites;
unordered_map<string, Shader*> Asset::Shaders;
unordered_map<string, Material*> Asset::Materials;

void Asset::Initialize(
    unordered_map<string, string>& texture_info,
    unordered_map<string, SpriteInfo>& sprite_info,
    unordered_map<string, Shader*>& shader_info,
    unordered_map<string, Material*>& material_info
) {
    for (auto& pair : texture_info) {
        const string& name = pair.first;
        const string& path = pair.second;
        Texture* texture = Texture::Load(path);
        if (texture == nullptr) {
            Log::Error("Failed to load texture: " + path);
            continue;
        }
        Textures[name] = texture;
    }

    for (auto& pair : sprite_info) {
        const string& name = pair.first;
        Texture* texture = Textures[name];
        if (texture == nullptr) {
            Log::Error("Failed to create sprite: texture not found for " + name);
            continue;
        }
        Sprites[name] = new Sprite(texture);
        Sprites[name]->pivot_x = pair.second.pivot_x;
        Sprites[name]->pivot_y = pair.second.pivot_y;
    }

    for (auto& pair : shader_info) {
        const string& name = pair.first;
        Shaders[name] = pair.second;
    }

    for (auto& pair : material_info) {
        const string& name = pair.first;
        Materials[name] = pair.second;
    }
}

void Asset::Clear() {
    for (auto& pair : Textures) {
        delete pair.second;
    }
    Textures.clear();

    for (auto& pair : Sprites) {
        delete pair.second;
    }
    Sprites.clear();

    for (auto& pair : Shaders) {
        delete pair.second;
    }
    Shaders.clear();

    for (auto& pair : Materials) {
        delete pair.second;
    }
    Materials.clear();
}
