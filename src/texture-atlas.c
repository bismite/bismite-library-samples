#include "common-core.h"
#include "common-ext.h"
#include <stdlib.h>

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

    // position
    node->x = x;
    node->y = y;
    node->w = tw;
    node->h = th;

    // color
    bi_set_color( node->color, 0xFF, 0xFF, 0xFF, 0xFF);
    return node;
}

static void world_create(BiContext* context)
{
    // texture
    BiTextureImage *img = malloc(sizeof(BiTextureImage));
    bi_load_texture("assets/tile.png",img,false,0);

    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    const int NEW_NODE = (context->w/32) * (context->h/32);

    for(int i=0; i<NEW_NODE; i++){
      int x = i % (context->w/32) * 32;
      int y = i / (context->w/32) * 32;

      BiNode* node = create_new_node(x,y,img);
      bi_add_node(root,node);
    }

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
    bi_init_context(context, 640, 480, 0, __FILE__, world_create);
    bi_start_run_loop(context);
    return 0;
}
