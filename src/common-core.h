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


static void print_stats(BiNode* node, void* _context, void* callback_context, double delta)
{
  BiContext *context = _context;
  double now = context->profile.frame_start_at;
  static double stats_shows_at = 0;
  if( context->debug && (now - stats_shows_at) > 1000 ) {
    LOG("FPS: %d/%.2f - frame: %.2f/%.2f [ms] - matrix_updated(AVG): %.2f - queued nodes: %d\n",
      context->profile.stats.actual_fps,
      context->profile.stats.estimated_fps,
      context->profile.stats.average_in_frame,
      context->profile.stats.average_frame_to_frame,
      context->profile.stats.total_matrix_updated / (double)(context->profile.stats.actual_fps),
      context->rendering_nodes_queue_size
    );
    stats_shows_at = now;
  }
}

static void __attribute__ ((unused)) enable_debug(BiContext *context)
{
  context->debug = true;
  context->on_update_callbacks[0] = print_stats;
  context->on_update_callbacks_size = 1;
}
