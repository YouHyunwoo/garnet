#pragma once

#include <unordered_map>
#include <string>
#include "sprite.h"

class Texture;
class Sprite;
class Shader;

class Asset {
public:
    static void Initialize(
        std::unordered_map<std::string, std::string>& texture_info,
        std::unordered_map<std::string, SpriteInfo>& sprite_info,
        std::unordered_map<std::string, Shader*>& shader_info
    );
    static void Clear();
    static Texture* GetTexture(const std::string& name);
    static Sprite* GetSprite(const std::string& name);
    static Shader* GetShader(const std::string& name);

    static std::unordered_map<std::string, Texture*> textures;
    static std::unordered_map<std::string, Sprite*> sprites;
    static std::unordered_map<std::string, Shader*> shaders;
};
