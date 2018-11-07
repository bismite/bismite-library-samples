#include "common-core.h"

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
    BiNode* face = face_sprite(0);
    face->x = context->w/2;
    face->y = context->h/2;
    bi_set_color( face->color, 0xFF, 0xFF, 0xFF, 0xFF);
    // add to scene graph
    bi_add_node(root,face);

    // add timer
    BiTimer *timer = malloc(sizeof(BiTimer));
    bi_timer_init(timer, rotate_face, 1000, 3, face);
    bi_node_add_timer(face,timer);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;
}

int main(int argc,char* argv[])
{
    BiContext _context;
    BiContext* context = &_context;
    bi_init_context(context, 480, 320, 0, false, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
