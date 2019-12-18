#include "common-core.h"

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // texture node
    BiNode* face = make_sprite("assets/face01.png");
    bi_node_set_position(face,context->w/2,context->h/2);
    bi_add_node(root,face);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;
    layer->textures[0] = face->texture->texture_image;
}

int main(int argc, char* argv[])
{
    print_version();
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, true, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
