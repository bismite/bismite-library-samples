#include "common-core.h"
#include "common-ext.h"

static bool random_text(double now,BiTimer* timer)
{
  static int i = 0;
  static const char* texts[] = {
    "ABCDEF",
    "1234567890",
    ";->",
    "(^_^) (^o^) ('q`)",
  };

  i = (i+1) % 4;

  printf("text: %s\n", texts[i]);
  BiNode *node = timer->userdata;
  BiFontAtlas *font = node->userdata;
  bi_update_label(node, texts[i], font);
  return true;
}

void world_create(BiContext* context)
{
    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);

    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);
    layer->root = root;

    // font
    BiFontAtlas *font = load_font();
    layer->textures[0] = font->texture_image;

    // label
    BiNode* label = malloc(sizeof(BiNode));
    bi_node_init(label);
    bi_set_color(label->color,32,32,32,0xff);
    label->scale_x = label->scale_y = 2.0;
    label->anchor_x = label->anchor_y = 0.5;
    bi_node_set_position( label, context->w/2, context->h/2 );
    label->userdata = font;
    bi_update_label(label, "(^-^)", font);

    bi_add_node(root,label);

    // add timer
    BiTimer *timer = malloc(sizeof(BiTimer));
    bi_timer_init(timer, random_text, 1500, -1, label);
    bi_node_add_timer(label,timer);

    // fps layer
    add_fps_layer(context,font);
}

int main(int argc, char* argv[])
{
    BiContext _context;
    BiContext* context = &_context;
    bi_init_context(context, 480, 320, 0, true, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
