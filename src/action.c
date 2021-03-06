#include <bi/ext/action.h>
#include "common-core.h"

struct ActionsContainer {
  BiAction* actions[0xFF];
  size_t num;
};

static void callback_nothing_to_do(BiAction *action, void* context)
{
  printf("Finish %p\n",action);
}

static void callback_change_color(BiAction* action, void* payload)
{
  BiNode *node = action->node;
  bi_set_color(node->color, rand()%0xFF, rand()%0xFF, rand()%0xFF, 0xFF);
  printf("CALLBACK: Node:%p Action:%p (%d,%d)\n", node, action, node->x, node->y);
}

static BiAction* alloc_action(size_t size, void* func, void* payload)
{
  BiAction* action = malloc(sizeof(BiAction));
  bi_action_init(action);
  action->timer = malloc(sizeof(BiTimer));
  action->action_data = malloc(size);
  action->on_finish = func;
  action->on_finish_callback_context = payload;
  printf("Alloc: %p %p %p\n", action, action->timer, action->action_data);
  return action;
}

static void dealloc_action(BiAction *action)
{
  printf("Dealloc: %p %p %p\n", action, action->timer, action->action_data);
  free(action->action_data);
  free(action->timer);
  free(action);
}

static void add_action(BiNode *node)
{
  // move, call, move
  BiAction* call0 = alloc_action( 0, callback_change_color, node );
  bi_action_base_init(call0);
  BiAction* move1 = alloc_action( sizeof(BiActionMoveTo), callback_nothing_to_do, NULL );
  bi_action_move_to_init(move1,1000,0,0);
  BiAction* call1 = alloc_action( 0, callback_change_color, node );
  bi_action_base_init(call1);
  BiAction* move2 = alloc_action( sizeof(BiActionMoveTo), callback_nothing_to_do, NULL );
  bi_action_move_to_init(move2,1000,480,320);
  BiAction* call2 = alloc_action( 0, callback_change_color, node );
  bi_action_base_init(call2);
  // sequence
  BiAction* actions[] = { call0, move1, call1, move2, call2 };
  BiAction* seq = alloc_action( sizeof(BiActionSequence), callback_nothing_to_do, NULL );
  bi_action_sequence_init(seq,5,actions);
  // repeat
  BiAction* repeat = alloc_action( sizeof(BiActionRepeat), callback_nothing_to_do, NULL );
  bi_action_repeat_init(repeat,seq);
  // add action to node
  bi_add_action(node,repeat);
  bi_action_start(node,repeat,bi_get_now());

  // add rotate action
  BiAction* rot = alloc_action( sizeof(BiActionRotateBy), callback_nothing_to_do, NULL );
  bi_action_rotate_by_init(rot,2000,360);
  bi_add_action(node,rot);
  bi_action_start(node,rot,bi_get_now());

  struct ActionsContainer *container = malloc(sizeof(struct ActionsContainer));
  container->actions[7] = rot;
  container->actions[6] = repeat;
  container->actions[5] = seq;
  container->actions[4] = call2;
  container->actions[3] = move2;
  container->actions[2] = call1;
  container->actions[1] = move1;
  container->actions[0] = call0;
  container->num = 8;
  node->userdata = container;
}

static void remove_action(BiNode *node)
{
  struct ActionsContainer *container = node->userdata;
  for(int i=0;i<container->num;i++){
    bi_remove_action(node,container->actions[i]);
    dealloc_action(container->actions[i]);
  }
  free(container->actions);
  node->userdata = NULL;
}

static bool on_click(BiContext* context,BiNode* n, int x, int y, int button, bool pressed)
{
  if(pressed==false) {
    if(n->userdata != NULL) {
      remove_action(n);
    }else{
      bi_node_set_position(n,240,160);
      add_action(n);
    }
  }
  return true;
}

static void world_create(BiContext* context)
{
    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);

    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);
    layer->root = root;

    // texture node
    BiNode* face = face_sprite();
    bi_node_set_position(face,240,160);
    bi_node_add_node(root,face);
    layer->textures[0] = face->texture_mapping->texture;

    // callback
    bi_node_set_on_click(face, on_click);

    // action
    add_action(face);

    bi_add_layer(context,layer);
}

int main(int argc, char* argv[])
{
    print_version();
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, false, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
