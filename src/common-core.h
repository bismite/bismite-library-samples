#include <bi/util.h>
#include <bi/node.h>
#include <bi/context.h>
#include <bi/main_loop.h>
#include <bi/texture.h>
#include <bi/layer.h>
#include <bi/logger.h>

#include <stdlib.h>
#include <stdio.h>

static BiNode* make_sprite(const char* name)
{
  // load texture
  BiTexture *texture = malloc(sizeof(BiTexture));
  BiTextureImage *img = malloc(sizeof(BiTextureImage));
  if( bi_load_texture(name,img,false) ) {
    texture->texture_image = img;
    bi_set_texture_boundary(texture,0,0,texture->texture_image->w,texture->texture_image->h);
  }else{
    LOG("load error\n");
    return NULL;
  }

  BiNode* face = malloc(sizeof(BiNode));
  bi_node_init(face);
  face->w = texture->w;
  face->h = texture->h;
  face->anchor_x = 0.5;
  face->anchor_y = 0.5;
  face->texture = texture;
  bi_set_color( face->color, 0xFF, 0xFF, 0xFF, 0xFF);

  return face;
}

static BiNode* face_sprite() __attribute__((unused)) ;
static BiNode* face_sprite()
{
  return make_sprite("assets/face01.png");
}
