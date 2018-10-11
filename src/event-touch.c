#include "common-core.h"
#include "common-ext.h"

static bool on_click(BiNode* n, void *context, int x, int y, int button, bool pressed)
{
  if(pressed) {
    bi_node_set_angle(n,n->angle + 30*3.1415/180.0);
    bi_node_set_position(n,x,y);
  }
  return true;
}

static bool on_move_cursor(BiNode* n, void *context, int x, int y)
{
    printf("on_move_cursor %d,%d\n",x,y);
    bi_node_set_position(n,x,y);
    return true;
}

#ifdef __EMSCRIPTEN__
static bool on_touch(BiNode* n, void *context, float x, float y, bool pressed)
{
  if(pressed) {
    printf("on_move_finger %.2f,%.2f -> %d,%d\n",x,y, (int)(x*480), (int)(y*320) );
    bi_node_set_position(n,x*480,y*320);
    bi_node_set_angle(n,n->angle + 30*3.1415/180.0);
  }
  return true;
}

static bool on_move_finger(BiNode* n, void* context, float x, float y)
{
    printf("on_move_finger %.2f,%.2f -> %d,%d\n",x,y, (int)(x*480), (int)(y*320) );
    bi_node_set_position(n,x*480, y * 320);
    return true;
}
#endif

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // sprite
    BiNode* node = face_sprite(0);
    bi_node_set_position(node,context->w/2,context->h/2);
    bi_add_node(root,node);

    // set callbacks
    bi_set_on_click(node, on_click, NULL);
    bi_set_on_move_cursor(node, on_move_cursor, NULL);
#ifdef __EMSCRIPTEN__
    bi_set_on_move_finger(node, on_move_finger, NULL);
    bi_set_on_touch(node, on_touch, NULL);
#endif

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
    bi_init_context(context, 480, 320, 0, "Event", world_create);
    bi_start_run_loop(context);
    return 0;
}
