// Created with TexturePacker (http://www.codeandweb.com/texturepacker)
//{% load cleanvarname %}{% load uppercase %}{% load objectinfo %}
// Sprite sheet: {{texture.fullName}} ({{texture.size.width}} x {{texture.size.height}})
//
// {{smartUpdateKey}}

#include <pebble.h>
#include "texture_packer.h"
#include "spritesheet.h"

static GBitmap* textures[TEXTURE_COUNT];
GBitmap* gbitmap_create_with_sprite(SpriteId sprite_id) {
  TextureId texture = 0;
  uint8_t total = 0;
  while (sprite_id >= (total += sprites_per_texture[texture])) {
    texture++;
  }
  if (!textures[texture]) {
    ResourceId resource = texture_resources[texture];
    textures[texture] = gbitmap_create_with_resource(resource);
  }
  return gbitmap_create_as_sub_bitmap(textures[texture], sprite_rects[sprite_id]);
}

void texture_destroy(TextureId texture_id) {
  if (textures[texture_id]) {
    gbitmap_destroy(textures[texture_id]);
    textures[texture_id] = NULL;
  }
}

void texture_destroy_all() {
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    texture_destroy(i);
  }
}

