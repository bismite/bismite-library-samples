#include "common-core.h"
#include "common-ext.h"
#include <stdlib.h>

static BiNode* create_new_node(int x, int y,BiTexture *img)
{
    BiNode* node = malloc(sizeof(BiNode));
    bi_node_init(node);

    // texture
    node->texture_mapping = malloc(sizeof(BiTextureMapping));
    bi_texture_mapping_init(node->texture_mapping);
    node->texture_mapping->texture = img;
    int tw = 32;
    int th = 32;
    int tx = rand()%(img->w/tw) * tw;
    int ty = rand()%(img->h/th) * th;
    bi_texture_mapping_set_bound(node->texture_mapping,tx,ty,tw,th);
    bi_node_set_position(node,x,y);
    bi_node_set_size(node,tw,th);
    bi_set_color( node->color, 0xFF, 0xFF, 0xFF, 0xFF);
    return node;
}

static void world_create(BiContext* context)
{
    // texture
    BiTexture *img = malloc(sizeof(BiTexture));
    bi_texture_init_with_filename(img,"assets/tile.png",false);

    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    const int NEW_NODE = (context->w/32) * (context->h/32);

    for(int i=0; i<NEW_NODE; i++){
      int x = i % (context->w/32) * 32;
      int y = i / (context->w/32) * 32;

      BiNode* node = create_new_node(x,y,img);
      bi_node_add_node(root,node);
    }

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;
    layer->textures[0] = img;

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
