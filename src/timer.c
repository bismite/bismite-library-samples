#include "common-core.h"
#include "common-ext.h"

static bool rotate_face(double now,BiTimer* timer)
{
  BiNode *node = timer->userdata;
  LOG("rotate_face %.2f\n", now);
  bi_node_set_angle(node, node->angle + 30 * M_PI/180);
  return true;
}

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // sprite node
    BiNode* face = face_sprite();
    face->x = context->w/2;
    face->y = context->h/2;
    bi_set_color( face->color, 0xFF, 0xFF, 0xFF, 0xFF);
    // add to scene graph
    bi_add_node(root,face);

    // add timer
    BiTimer *timer = malloc(sizeof(BiTimer));
    bi_timer_init(timer, rotate_face, 1000, 3, face);
    bi_add_timer(&face->timers,timer);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->textures[0] = face->texture->texture_image;
    bi_add_layer(context,layer);
    layer->root = root;

    // fps layer
    BiFontAtlas *font = load_font();
    add_fps_layer(context,font);
}

int main(int argc,char* argv[])
{
  print_version();
  BiContext* context = malloc(sizeof(BiContext));
  bi_init_context(context, 480, 320, 0, false, __FILE__);
  world_create(context);
  bi_start_run_loop(context);
  return 0;
}
