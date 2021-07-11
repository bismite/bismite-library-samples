#include "common-core.h"
#include "common-ext.h"
#include <stdlib.h>

static BiNode* create_new_node(int x, int y)
{
    BiNode* node = malloc(sizeof(BiNode));
    bi_node_init(node);
    node->anchor_x = node->anchor_y = 0.5;
    bi_node_set_position(node,x,y);
    bi_node_set_size(node,50,50);
    bi_set_color( node->color, rand()%0xFF, rand()%0xFF, rand()%0xFF, 0xFF);
    return node;
}

static bool on_click(BiContext* context,BiNode* n, int x, int y, int button, bool pressed)
{
  if(pressed) {
    int lx,ly;
    bi_node_transform_local(n,x,y,&lx,&ly);
    if( bi_node_inside(n,x,y) ){
      LOG("HIT: %d,%d -> %d,%d\n", x,y, lx,ly);
      bi_node_set_angle(n,n->angle + 30 * M_PI / 180.0);
      return true; // swallow event
    }
  }

  return false; // not swallow event
}

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // hit rect
    for(int i=0;i<20;i++){
      int x = rand()%context->w;
      int y = rand()%context->h;
      BiNode* node = create_new_node(x,y);
      bi_node_set_on_click(node, on_click);
      bi_node_add_node(root,node);
    }

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;

    // fps layer
    BiFontAtlas *font = load_font();
    add_fps_layer(context,font);
}

int main(int argc,char* argv[])
{
    print_version();
    srand( bi_get_now() );
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, true, __FILE__);
    print_gl_info();
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
