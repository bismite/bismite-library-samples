#include "common-core.h"
#include "common-ext.h"
#include <stdlib.h>

static void spin(BiNode* node, void *context, void *callback_context, double delta)
{
    bi_node_set_angle(node, node->angle + 2*3.1415/180);
}

static void world_create(BiContext* context)
{
    context->debug = true;
    bi_set_color(context->color,0xff,32,128,0xff);

    //
    // face sprite layer
    //
    // sprite
    BiNode *sprite = make_sprite("assets/face01.png",0);
    sprite->scale_x = sprite->scale_y = 2.0;
    bi_node_set_position(sprite,context->w/2,context->h/2);
    bi_set_color(sprite->color, 0xff, 0xff, 0xff, 0xff);
    // shadow
    BiNode *shadow = malloc(sizeof(BiNode));
    bi_node_init(shadow);
    shadow->w = context->w;
    shadow->h = context->h;
    bi_set_color(shadow->color, 0, 0, 0, 128);
    // root
    BiNode *root = malloc(sizeof(BiNode));
    bi_node_init(root);
    bi_add_node(root,sprite);
    bi_add_node(root,shadow);
    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->root = root;
    bi_add_layer(context,layer);

    //
    // spotlight layer
    //

    // sprite
    BiNode *spotlight = make_sprite("assets/circle256.png",1);
    bi_node_set_position(spotlight,context->w/2,context->h/2);
    bi_set_color(spotlight->color, 0xff, 0xff, 0xff, 0xff);
    // spin sprite!
    bi_set_on_update(spotlight,spin,NULL);
    // spotlight layer
    BiLayer *spotlight_layer = malloc(sizeof(BiLayer));
    bi_layer_init(spotlight_layer);
    spotlight_layer->root = spotlight;
    bi_add_layer(context,spotlight_layer);
    // spotlight blend setting
    spotlight_layer->blend_src = GL_DST_COLOR;
    spotlight_layer->blend_dst = GL_ONE;

    //
    // fps layer
    //
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
