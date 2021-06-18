#include "common-core.h"
#include "common-ext.h"

#include "shader-vertex.h"
#include "shader-wave.h"
#include "shader-monochrome.h"

int main(int argc, char* argv[])
{
    print_version();
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, true, __FILE__);

    // shaders
    BiShader *wave_shader = malloc(sizeof(BiShader));
    BiShader *mono_shader = malloc(sizeof(BiShader));

    bi_shader_init(wave_shader,context->w,context->h, VERTEX_SHADER, WAVE_FRAGMENT_SHADER);
    bi_shader_init(mono_shader,context->w,context->h, VERTEX_SHADER, MONOCHROME_FRAGMENT_SHADER);

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
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->root = root;
    layer->shader = mono_shader;
    layer->textures[0] = texture_mapping->texture;

    // post processing - wave shader
    context->post_processing.shader = wave_shader;

    //
    bi_add_layer(context,bg_layer);
    bi_add_layer(context,layer);

    // fps layer
    BiFontAtlas *font = load_font();
    add_fps_layer(context,font);

    //
    bi_start_run_loop(context);
    return 0;
}
