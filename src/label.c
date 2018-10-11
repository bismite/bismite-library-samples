#include "common-core.h"
#include "common-ext.h"

static bool random_text(BiContext* context,BiNode* node,double now,BiTimer* timer)
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
  BiFontAtlas *font = node->userdata;
  bi_set_color(font->color, rand()%0xff, rand()%0xff, rand()%0xff, 0xff);
  bi_update_label(node, texts[i], font);
  return true;
}

void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // label
    BiTextureImage *font_img = malloc(sizeof(BiTextureImage));
    bi_load_texture("assets/gohufont.png",font_img,false,0);
    // load layout
    BiFontAtlas *font = malloc(sizeof(BiFontAtlas));
    font->texture_image = font_img;
    bi_set_color(font->color,0xff,0xff,0xff,0xff);
    const char* layout_file_name = "assets/gohufont-bold-14-0.0.dat";
    bi_load_font_layout(layout_file_name,font);

    // label
    BiNode* label = malloc(sizeof(BiNode));
    bi_node_init(label);
    bi_node_set_position(label,0+10,320-10);
    bi_set_color(label->color,0xff,0,0,0xff);
    label->scale_x = 3.0;
    label->scale_y = 3.0;
    label->anchor_y = 1.0;
    label->userdata = font;
    bi_update_label(label, "(^-^)", font);

    bi_add_node(root,label);

    // add timer
    BiTimer *timer = malloc(sizeof(BiTimer));
    bi_timer_init(timer, label, random_text, 1500, -1, NULL);
    bi_add_timer(context,timer);

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
