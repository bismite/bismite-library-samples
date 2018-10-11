#include <bi/ext/font.h>
#include <bi/layer.h>
#include <stdio.h>
#include <stdlib.h>

static bool label_fps_indicate(BiContext* context,BiNode* node,double now,BiTimer* timer)
{
  BiFontAtlas *font = node->userdata;
  char text[1024];
  snprintf(text,1024,"FPS:%.2f", context->profile.current_fps);
  bi_update_label(node, text, font);
  return true;
}

static BiNode* create_fps_label(BiContext* context, int texture_unit)
{
    BiTextureImage *font_img = malloc(sizeof(BiTextureImage));
    bi_load_texture("assets/gohufont.png",font_img,true,texture_unit);

    // load layout
    BiFontAtlas *font = malloc(sizeof(BiFontAtlas));
    font->texture_image = font_img;
    bi_set_color(font->color,0xff,0xff,0xff,0xff);
    const char* layout_file_name = "assets/gohufont-bold-14-0.0.dat";
    bi_load_font_layout(layout_file_name,font);

    // label
    BiNode* label = malloc(sizeof(BiNode));
    bi_node_init(label);
    label->userdata = font;
    bi_update_label(label, "FPS:00.00", font);

    // timer
    BiTimer *timer = malloc(sizeof(BiTimer));
    bi_timer_init(timer, label, label_fps_indicate, 1000, -1, NULL);
    bi_add_timer(context,timer);

    return label;
}

static void __attribute__((unused))add_fps_layer(BiContext* context)
{
    // label
    BiNode* label = create_fps_label(context,1);
    label->anchor_x = 0;
    label->anchor_y = 1;
    label->x = 0;
    label->y = context->h;
    // layer
    BiLayer *text_layer = malloc(sizeof(BiLayer));
    bi_layer_init(text_layer);
    text_layer->root = label;
    bi_add_layer(context,text_layer);
}
