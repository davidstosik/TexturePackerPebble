// Created with TexturePacker (http://www.codeandweb.com/texturepacker)
//{% load cleanvarname %}{% load uppercase %}
// Sprite sheet: {{texture.fullName}} ({{texture.size.width}} x {{texture.size.height}})
//
// {{smartUpdateKey}}
#pragma once
#include <pebble.h>
#include "texture_packer.h"

enum {
  INVALID_TEXTURE_ID = -1,{% for result in allResults %}{% for texture in result.textures %}
  TEXTURE_ID_{{texture.trimmedName|cleanvarname|uppercase}},{% endfor %}{% endfor %}
  TEXTURE_COUNT
};

enum {
  INVALID_SPRITE_ID = -1,{% for result in allResults %}{% for texture in result.textures %}{% for sprite in texture.allSprites %}
  SPRITE_ID_{{sprite.trimmedName|cleanvarname|uppercase}},{% endfor %}{% endfor %}{% endfor %}
};

static const ResourceId texture_resources[] = {{% for result in allResults %}{% for texture in result.textures %}
  RESOURCE_ID_{{texture.trimmedName|cleanvarname|uppercase}},{% endfor %}{% endfor %}
};

static const uint16_t sprites_per_texture[] = {{% for result in allResults %}{% for texture in result.textures %}
  {{texture.allSprites.count}},{% endfor %}{% endfor %}
};

static const GRect sprite_rects[] = {{% for result in allResults %}{% for texture in result.textures %}{% for sprite in texture.allSprites %}
  { { {{sprite.frameRect.x}} ,  {{sprite.frameRect.y}} }, { {{sprite.frameRect.width}}, {{sprite.frameRect.height}} } },{% endfor %}{% endfor %}{% endfor %}
};

