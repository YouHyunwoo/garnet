#include "sprite_renderer.h"

#include "../log.h"
#include "../graphic.h"
#include "../sprite.h"
#include "../object.h"
#include "../texture.h"
#include "../shader.h"
#include "../fragment_input.h"
#include "../material.h"
#include "../vector.h"
#include "transform.h"

SpriteRenderer::SpriteRenderer(Sprite* sprite, Material* material, double z_index)
: sprite(sprite), z_index(z_index) {
    if (material == nullptr) {
        material = new Material(nullptr);
    }
    materials.push_back(material);
}

void SpriteRenderer::Render(Graphic& graphic) {
    if (sprite == nullptr) { return; }

    Texture* texture = sprite->texture;

    Color color;

    FragmentInput frag_input;
    frag_input.object = object;

    Vector3 global_position;
    object->transform->GetGlobalPosition(global_position);

    for (auto& material : materials) {
        Shader* shader = material->shader;

        frag_input.data = shader == nullptr ? nullptr : shader->data;
        frag_input.material = material;

        int sprite_offset_x = -static_cast<int>(sprite->pivot_x * texture->width);
        int sprite_offset_y = -static_cast<int>(sprite->pivot_y * texture->height);

        for (int r = 0; r < texture->height; r++) {
            for (int c = 0; c < texture->width; c++) {
                int local_x = sprite_offset_x + c;
                int local_y = sprite_offset_y + r;
                int screen_x = static_cast<int>(global_position.x) + local_x;
                int screen_y = static_cast<int>(global_position.y) + local_y;
                if (shader) {
                    if (!graphic.IsDrawable(screen_x, screen_y, z_index)) { continue; }

                    frag_input.screen_x = screen_x;
                    frag_input.screen_y = screen_y;
                    frag_input.local_x = local_x;
                    frag_input.local_y = local_y;
                    frag_input.x = c;
                    frag_input.y = r;
                    frag_input.u = static_cast<double>(c) / (texture->width - 1);
                    frag_input.v = static_cast<double>(r) / (texture->height - 1);

                    color = shader->Shade(frag_input);
                }
                else {
                    texture->GetColor(c, r, color);
                }

                graphic.DrawPoint(screen_x, screen_y, color);
            }
        }
    }
}