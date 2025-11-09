#pragma once

#include <unordered_map>
#include <string>
#include "sprite.h"

class Texture;
class Sprite;
class Shader;
class Material;

class Asset {
public:
    static std::unordered_map<std::string, Texture*> Textures;
    static std::unordered_map<std::string, Sprite*> Sprites;
    static std::unordered_map<std::string, Shader*> Shaders;
    static std::unordered_map<std::string, Material*> Materials;

    static void Initialize(
        std::unordered_map<std::string, std::string>& texture_info,
        std::unordered_map<std::string, SpriteInfo>& sprite_info,
        std::unordered_map<std::string, Shader*>& shader_info,
        std::unordered_map<std::string, Material*>& material_info
    );
    static void Clear();
};
