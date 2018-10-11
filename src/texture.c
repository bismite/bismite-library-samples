#include "common-core.h"

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // texture node
    BiNode* face = face_sprite(0);
    bi_node_set_position(face,context->w/2,context->h/2);
    bi_add_node(root,face);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;
}

int main(int argc, char* argv[])
{
    BiContext _context;
    BiContext* context = &_context;
    bi_init_context(context, 480, 320, 0, __FILE__, world_create);
    bi_start_run_loop(context);
    return 0;
}
