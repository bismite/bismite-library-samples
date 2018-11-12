#include <bi/ext/font.h>
#include <bi/layer.h>
#include <stdio.h>
#include <stdlib.h>

struct _labelUpdateContext{
  BiNode *node;
  BiContext *context;
};

static bool label_fps_indicate(double now,BiTimer* timer)
{
  struct _labelUpdateContext *label_update_context = timer->userdata;
  BiNode *node = label_update_context->node;
  BiContext *context = label_update_context->context;
  BiFontAtlas *font = node->userdata;
  char text[1024];
  snprintf(text,1024,"FPS:%.2f", context->profile.current_fps);
  bi_update_label(node, text, font);
  return true;
}

static BiFontAtlas* load_font()
{
  // texture
  BiTextureImage *font_img = malloc(sizeof(BiTextureImage));
  bi_load_texture("assets/gohufont.png",font_img,true);
  // layout
  BiFontAtlas *font = malloc(sizeof(BiFontAtlas));
  font->texture_image = font_img;
  bi_set_color(font->color,0xff,0xff,0xff,0xff);
  const char* layout_file_name = "assets/gohufont-bold-14-0.0.dat";
  bi_load_font_layout(layout_file_name,font);
  return font;
}

static BiNode* create_fps_label(BiContext* context, BiFontAtlas *font)
{
    // label
    BiNode* label = malloc(sizeof(BiNode));
    bi_node_init(label);
    label->userdata = font;
    bi_update_label(label, "FPS:00.00", font);

    // timer
    BiTimer *timer = malloc(sizeof(BiTimer));
    struct _labelUpdateContext *label_update_context = malloc(sizeof(struct _labelUpdateContext));
    label_update_context->node = label;
    label_update_context->context = context;
    bi_timer_init(timer, label_fps_indicate, 1000, -1, label_update_context);
    bi_node_add_timer(label,timer);

    return label;
}

static void __attribute__((unused))add_fps_layer(BiContext* context,BiFontAtlas *font)
{
    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->textures[0] = font->texture_image;

    // label
    BiNode* label = create_fps_label(context,font);
    label->anchor_x = 0;
    label->anchor_y = 1;
    label->x = 0;
    label->y = context->h;

    layer->root = label;
    bi_add_layer(context,layer);
}
