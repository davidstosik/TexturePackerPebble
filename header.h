// Created with TexturePacker (http://www.codeandweb.com/texturepacker)
//{% load cleanvarname %}{% load uppercase %}{% load firstcharletter %}
// Sprite sheet: {{texture.fullName}} ({{texture.size.width}} x {{texture.size.height}})
//
// {{smartUpdateKey}}

#include <pebble.h>

typedef enum {
  INVALID_SPRITE = 0,{% for sprite in allSprites %}
  SPRITE_ID_{{sprite.trimmedName|cleanvarname|uppercase}},{% endfor %}
} SpriteId;

GBitmap* gbitmap_create_with_sprite(SpriteId sprite_id);
void destroy_sprite();
