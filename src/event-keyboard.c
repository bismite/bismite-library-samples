#include "common-core.h"
#include "common-ext.h"
#include <bi/bi_sdl.h>

static bool on_keyinput(BiNode* node, void *context, uint16_t scancode, uint32_t keycode, uint16_t mod, bool pressed)
{
  BiNode* labels = context;
  BiNode* label;
  char buf[256];

  snprintf(buf, 256, "Keycode: %s (%d)", SDL_GetKeyName(keycode), keycode);
  label = &labels[1];
  bi_update_label(label, buf, label->userdata);

  snprintf(buf, 256, "KeycodeFromScancode: %s (%d)", SDL_GetKeyName(SDL_GetKeyFromScancode(scancode)), SDL_GetKeyFromScancode(scancode) );
  label = &labels[2];
  bi_update_label(label, buf, label->userdata);

  snprintf(buf, 256, "Scancode: %s (%d)", SDL_GetScancodeName(scancode), scancode );
  label = &labels[3];
  bi_update_label(label, buf, label->userdata);

  snprintf(buf, 256, "MOD: %d", mod );
  label = &labels[4];
  bi_update_label(label, buf, label->userdata);

  snprintf(buf, 256, "Pressed: %s", pressed?"DOWN":"UP" );
  label = &labels[5];
  bi_update_label(label, buf, label->userdata);

  return false;
}

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // font
    BiFontAtlas *font = load_font();

    // labels
    BiNode* labels = malloc(sizeof(BiNode)*6);
    for(int i=0;i<6;i++){
      BiNode *label = &labels[i];
      bi_node_init(label);
      label->scale_x = label->scale_y = 1.0;
      bi_node_set_position( label, 10, context->h - 100 - i * 32 );
      label->userdata = font;
      bi_add_node(root,label);
    }

    bi_update_label(&labels[0], "PRESS ANY KEY", font);
    bi_update_label(&labels[1], "Keycode:", font);
    bi_update_label(&labels[2], "KeycodeFromScancode:", font);
    bi_update_label(&labels[3], "Scancode:", font);
    bi_update_label(&labels[4], "MOD:", font);
    bi_update_label(&labels[5], "Pressed:", font);

    // set callback
    bi_set_on_keyinput(root, on_keyinput, labels);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;
    layer->textures[0] = font->texture_image;

    // fps layer
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
