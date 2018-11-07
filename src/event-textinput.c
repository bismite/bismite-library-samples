#include "common-core.h"
#include "common-ext.h"

static bool on_textinput(BiNode* n, void *context, char* text)
{
  printf("%s\n",text);
  return true;
}

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
    bi_set_on_textinput(node, on_textinput, NULL);

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
    bi_init_context(context, 480, 320, 0, false, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
