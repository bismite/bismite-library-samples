#include "common-core.h"
#include "common-ext.h"

#include "shader-vertex.h"
#include "shader-wave.h"
#include "shader-monochrome.h"
#include "shader-pixelate.h"

int main(int argc, char* argv[])
{
    print_version();
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, true, __FILE__);

    // layer group
    BiLayerGroup* layer_group = malloc(sizeof(BiLayerGroup));
    bi_layer_group_init(layer_group);

    // shaders
    BiShader *mono_shader = malloc(sizeof(BiShader));
    bi_shader_init(mono_shader,context->w,context->h, VERTEX_SHADER, MONOCHROME_FRAGMENT_SHADER);
    BiShader *wave_shader = malloc(sizeof(BiShader));
    bi_shader_init(wave_shader,context->w,context->h, VERTEX_SHADER, WAVE_FRAGMENT_SHADER);
    BiShader *pixelate_shader = malloc(sizeof(BiShader));
    bi_shader_init(pixelate_shader,context->w,context->h, VERTEX_SHADER, PIXELATE_FRAGMENT_SHADER);

    // Background layer - default shader
    BiLayer *bg_layer = malloc(sizeof(BiLayer));
    bi_layer_init(bg_layer);
    bg_layer->root = malloc(sizeof(BiNode));
    bi_node_init(bg_layer->root);
    BiNode* sky = make_sprite("assets/sky.png");
    bi_node_set_position(sky,context->w/2,context->h/2);
    bi_node_add_node(bg_layer->root,sky);
    bg_layer->textures[0] = sky->texture_mapping->texture;

    // foreground root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // foreground textures
    BiTextureMapping* texture_mapping = make_texture_mapping("assets/face01.png");
    for(int i=0; i<4; i++){
      BiNode* face = make_sprite_from_mapping(texture_mapping);
      int x = (1+i%2) * context->w/3;
      int y = (1+i/2) * context->h/3;
      bi_node_set_position(face,x,y);
      bi_node_add_node(root,face);
    }

    // foreground layer - monochrome shader
    BiLayer *fg_layer = malloc(sizeof(BiLayer));
    bi_layer_init(fg_layer);
    fg_layer->root = root;
    fg_layer->shader = mono_shader;
    fg_layer->textures[0] = texture_mapping->texture;

    // post processing - wave shader
    BiPostProcess* post_process_1 = malloc(sizeof(BiPostProcess));
    BiPostProcess* post_process_2 = malloc(sizeof(BiPostProcess));
    bi_post_process_init(post_process_1);
    bi_post_process_init(post_process_2);
    post_process_1->shader = wave_shader;
    post_process_2->shader = pixelate_shader;
    bi_layer_group_add_post_process(layer_group,post_process_1);
    bi_layer_group_add_post_process(layer_group,post_process_2);

    //
    bi_layer_group_add_layer(layer_group,bg_layer);
    bi_layer_group_add_layer(layer_group,fg_layer);
    bi_layer_group_add_layer_group(&context->layers,layer_group);

    // fps layer
    BiFontAtlas *font = load_font();
    add_fps_layer(context,font);

    //
    bi_start_run_loop(context);
    return 0;
}
