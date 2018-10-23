#include <bi/ext/action.h>
#include "common-core.h"

static bool on_click(BiNode* n, void *context, int x, int y, int button, bool pressed)
{
  if(n->userdata != NULL) {
    bi_remove_action(context,n,n->userdata);
    free(n->userdata);
    n->userdata = NULL;
  }
  return true;
}

static void world_create(BiContext* context)
{
    context->debug = true;

    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // texture node
    BiNode* face = face_sprite(0);
    bi_node_set_position(face,context->w/2,context->h/2);
    bi_add_node(root,face);

    // action
    BiAction* move1 = malloc(sizeof(BiAction));
    bi_action_move_to_init(move1,2000,0,0);
    BiAction* move2 = malloc(sizeof(BiAction));
    bi_action_move_to_init(move2,2000,context->w,context->h);
    BiAction* actions[] = {move1, move2};
    BiAction* seq = malloc(sizeof(BiAction));
    bi_action_sequence_init(seq,2,actions);

    // action
    BiAction* rep = malloc(sizeof(BiAction));
    bi_action_repeat_init(rep,seq);
    bi_add_action(context,face,rep);
    bi_action_start(face,rep,bi_get_now());

    // remove action when touch
    face->userdata = rep;
    bi_set_on_click(face, on_click, context);

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
