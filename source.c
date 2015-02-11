// Created with TexturePacker (http://www.codeandweb.com/texturepacker)
//{% load cleanvarname %}{% load uppercase %}{% load firstcharletter %}
// Sprite sheet: {{texture.fullName}} ({{texture.size.width}} x {{texture.size.height}})
//
// {{smartUpdateKey}}

#include <pebble.h>
#include "{{texture.trimmedName}}.h"

static const GRect sprite_rects[] = {
  { {0, 0}, {0, 0} },{% for sprite in allSprites %}
  { { {{sprite.frameRect.x}} ,  {{sprite.frameRect.y}} }, { {{sprite.frameRect.width}}, {{sprite.frameRect.height}} } },{% endfor %}
};

static GBitmap *sprite;
GBitmap* gbitmap_create_with_sprite(SpriteId sprite_id) {
  if (!sprite) sprite = gbitmap_create_with_resource(RESOURCE_ID_SPRITE);
  return gbitmap_create_as_sub_bitmap(sprite, sprite_rects[sprite_id]);
}

void destroy_sprite() {
  gbitmap_destroy(sprite);
}

