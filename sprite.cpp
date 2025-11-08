#include "sprite.h"

#include "graphic.h"

Sprite::Sprite(Texture* texture)
    : texture(texture) {}

void Sprite::Render(int x, int y, Graphic& graphic) {
    // graphic.DrawTexture(x - (int)(texture->width * pivot_x), y - (int)(texture->height * pivot_y), *texture, shader);
}
