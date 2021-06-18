#include "common-core.h"
#include "common-ext.h"
#include <stdlib.h>

struct event_context{
  BiContext *context;
  BiLayer *layer;
};

static BiNode* create_new_node(int x, int y,BiTexture *tex)
{
    BiNode* node = malloc(sizeof(BiNode));
    bi_node_init(node);

    // texture
    node->texture_mapping = malloc(sizeof(BiTextureMapping));
    bi_texture_mapping_init(node->texture_mapping);
    node->texture_mapping->texture = tex;
    int tw = 32;
    int th = 32;
    int tx = rand()%(tex->w/tw) * tw;
    int ty = rand()%(tex->h/th) * th;
    bi_texture_mapping_set_bound(node->texture_mapping,tx,ty,tw,th);
    bi_node_set_position(node,x,y);
    bi_node_set_size(node,tw,th);
    bi_set_color( node->color, 0xFF, 0xFF, 0xFF, 0xFF);
    return node;
}

static bool on_move_cursor(BiNode* n, void *context, int x, int y)
{
    struct event_context* c = context;
    int layer_w = 32*64 * 0.5;
    int layer_h = 32*64 * 0.5;
    c->layer->camera_x = (float)x / c->context->w * layer_w;
    c->layer->camera_y = (float)y / c->context->h * layer_h;
    return true;
}

#ifdef __EMSCRIPTEN__
static bool on_move_finger(BiNode* n, void* context, float x, float y, int64_t finger_id)
{
    return on_move_cursor(n, context, x, y);
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
    layer->z_order = -1;

    bi_add_layer(context,layer);

    // texture
    BiTexture *tex = malloc(sizeof(BiTexture));
    bi_texture_init(tex);
    bi_texture_load_from_file(tex,"assets/tile.png",false);
    layer->textures[0] = tex;

    // root node
    BiNode* tiles = malloc(sizeof(BiNode));
    bi_node_init(tiles);
    layer->root = tiles;
    tiles->scale_x = tiles->scale_y = 0.5;

    struct event_context *c = malloc(sizeof(struct event_context));
    c->context = context;
    c->layer = layer;

    bi_set_on_move_cursor(tiles, on_move_cursor, c);
#ifdef __EMSCRIPTEN__
    bi_set_on_move_finger(tiles, on_move_finger, c);
#endif

    for(int x=0; x<64; x++) {
      for(int y=0; y<64; y++){
        BiNode* node = create_new_node(x*32,y*32,tex);
        bi_node_add_node(tiles,node);
      }
    }
}

int main(int argc,char* argv[])
{
    print_version();
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, false, __FILE__);

    //
    // fps layer
    //
    BiFontAtlas *font = load_font();
    add_fps_layer(context,font);

    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
