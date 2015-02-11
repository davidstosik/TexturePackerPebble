// Created with TexturePacker (http://www.codeandweb.com/texturepacker)
//{% load cleanvarname %}{% load uppercase %}
// Sprite sheet: {{texture.fullName}} ({{texture.size.width}} x {{texture.size.height}})
//
// {{smartUpdateKey}}

#include <pebble.h>

typedef uint8_t TextureId;
typedef uint8_t SpriteId;

GBitmap* gbitmap_create_with_sprite(SpriteId sprite_id);
void texture_destroy(TextureId texture_id);
void texture_destroy_all();
