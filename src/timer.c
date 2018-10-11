#include "common-core.h"
#include "common-ext.h"

static bool rotate_face(BiContext* context,BiNode* node,double now,BiTimer* timer)
{
  printf("rotate_face %.2f\n", now);
  bi_node_set_angle(node, node->angle + 30 * 3.14159/180);
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
    bi_timer_init(timer, face, rotate_face, 1000, 3, NULL);
    bi_add_timer(context,timer);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;

    // fps layer
    add_fps_layer(context);
}

int main(int argc,char* argv[])
{
    BiContext _context;
    BiContext* context = &_context;
    bi_init_context(context, 480, 320, 0, __FILE__, world_create);
    bi_start_run_loop(context);
    return 0;
}
