#include "common-core.h"
#include "common-ext.h"
#include <stdlib.h>

struct event_context{
  BiContext *context;
  BiLayer *layer;
};

static BiNode* create_new_node(int x, int y,BiTextureImage *img)
{
    BiNode* node = malloc(sizeof(BiNode));
    bi_node_init(node);

    // texture
    node->texture = malloc(sizeof(BiTexture));
    node->texture->texture_image = img;
    int tw = 32;
    int th = 32;
    int tx = rand()%(img->w/tw) * tw;
    int ty = rand()%(img->h/th) * th;
    bi_set_texture_boundary(node->texture,tx,ty,tw,th);
    bi_node_set_position(node,x,y);
    bi_node_set_size(node,tw,th);
    bi_set_color( node->color, 0xFF, 0xFF, 0xFF, 0xFF);
    return node;
}

static bool on_move_cursor(BiNode* n, void *context, int x, int y)
{
    struct event_context* c = context;
    c->layer->camera_x = x;
    c->layer->camera_y = y;
    return true;
}

#ifdef __EMSCRIPTEN__
static bool on_move_finger(BiNode* n, void* context, float x, float y)
{
    struct event_context* c = context;
    c->layer->camera_x = x;
    c->layer->camera_y = y;
    return true;
}
#endif

static void world_create(BiContext* context)
{
    context->debug = true;
    bi_set_color(context->color,32,32,0,0xff);

    //
    // tile layer
    //
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->projection_centering = true;
    bi_add_layer(context,layer);

    // texture
    BiTextureImage *img = malloc(sizeof(BiTextureImage));
    bi_load_texture("assets/tile.png",img,false,0);

    // root node
    BiNode* tiles = malloc(sizeof(BiNode));
    bi_node_init(tiles);
    layer->root = tiles;
    tiles->scale_x = tiles->scale_y = 0.5;
    bi_node_set_position(tiles, context->w/2 -64*32/2*0.5, context->h/2 -64*32/2*0.5);

    struct event_context *c = malloc(sizeof(struct event_context));
    c->context = context;
    c->layer = layer;

    bi_set_on_move_cursor(tiles, on_move_cursor, c);
#ifdef __EMSCRIPTEN__
    bi_set_on_move_finger(tiles, on_move_finger, c);
#endif

    for(int x=0; x<64; x++) {
      for(int y=0; y<64; y++){
        BiNode* node = create_new_node(x*32,y*32,img);
        bi_add_node(tiles,node);
      }
    }

    //
    // fps layer
    //
    add_fps_layer(context);
}

int main(int argc,char* argv[])
{
    BiContext _context;
    BiContext* context = &_context;
    bi_init_context(context, 800, 600, 0, false, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
