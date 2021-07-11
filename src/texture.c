#include "common-core.h"

int main(int argc, char* argv[])
{
    print_version();
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, true, __FILE__);

    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // texture node 1
    BiNode* sky = make_sprite("assets/sky.png");
    bi_node_set_position(sky,context->w/2,context->h/2);
    bi_node_add_node(root,sky);

    // texture node 2
    BiNode* face = make_sprite("assets/face01.png");
    bi_node_set_position(face,context->w/2,context->h/2);
    bi_node_add_node(root,face);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;
    layer->textures[0] = sky->texture_mapping->texture;
    layer->textures[1] = face->texture_mapping->texture;

    bi_start_run_loop(context);
    return 0;
}
