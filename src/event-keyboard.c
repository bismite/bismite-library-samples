#include "common-core.h"
#include "common-ext.h"
#include <bi/bi_sdl.h>

static bool on_keyinput(BiNode* node, void *context, uint16_t scancode, uint32_t keycode, uint16_t mod, bool pressed)
{
  LOG("Keycode:%s(%d) KeycodeFromScancode:%s(%d) Scancode:%s(%d) MOD:%d %s\n",
    SDL_GetKeyName(keycode), keycode,
    SDL_GetKeyName(SDL_GetKeyFromScancode(scancode)), SDL_GetKeyFromScancode(scancode),
    SDL_GetScancodeName(scancode), scancode,
    mod,
    pressed?"DOWN":"UP");


  if(pressed==false){
    return false;
  }

  switch(keycode){
    case SDLK_UP:
      bi_node_set_position(node,node->x,node->y+10);
      break;
    case SDLK_DOWN:
      bi_node_set_position(node,node->x,node->y-10);
      break;
    case SDLK_LEFT:
      bi_node_set_position(node,node->x-10,node->y);
      break;
    case SDLK_RIGHT:
      bi_node_set_position(node,node->x+10,node->y);
      break;
  }

  return false;
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

    // set callback
    bi_set_on_keyinput(node, on_keyinput, NULL);

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
