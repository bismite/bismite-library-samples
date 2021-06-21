#include "common-core.h"
#include "common-ext.h"

#ifdef __EMSCRIPTEN__
#define SHADER_HEADER "#version 100\nprecision highp float;\n"
#else
#define SHADER_HEADER "#version 120\n"
#endif

char* read_shader(const char* filename)
{
  int header_size = strlen(SHADER_HEADER);
  FILE *file = fopen(filename,"rb");
  fseek(file, 0, SEEK_END);
  int file_size = ftell(file);
  rewind(file);
  char* string = malloc(file_size+header_size+1);
  strcpy(string,SHADER_HEADER);
  fread(string+header_size, sizeof(char), file_size, file);
  string[file_size+header_size] = 0;
  fclose(file);
  printf("* [%s]\n%s\n",filename,string);
  return string;
}

int main(int argc, char* argv[])
{
  print_version();
  BiContext* context = malloc(sizeof(BiContext));
  bi_init_context(context, 480, 320, 0, true, __FILE__);

  // Shaders
  char* vartex_shader = read_shader("assets/shader-vertex.vert");
  char* bg_shader_string = read_shader("assets/shader-bg.frag");
  char* fg_shader_string = read_shader("assets/shader-fg.frag");
  char* pp_shader_strings[2] = {
    read_shader("assets/shader-post-process-1.frag"),
    read_shader("assets/shader-post-process-2.frag")
  };
  BiShader *bg_shader = malloc(sizeof(BiShader));
  bi_shader_init(bg_shader,context->w,context->h, vartex_shader, bg_shader_string);
  BiShader *fg_shader = malloc(sizeof(BiShader));
  bi_shader_init(fg_shader,context->w,context->h, vartex_shader, fg_shader_string);
  BiShader *pp_shaders[2];
  for(int i=0;i<2;i++){
    pp_shaders[i] = malloc(sizeof(BiShader));
    bi_shader_init(pp_shaders[i],context->w,context->h, vartex_shader, pp_shader_strings[i]);
  }

  // Background layer
  BiLayer *bg_layer = malloc(sizeof(BiLayer));
  bi_layer_init(bg_layer);
  bg_layer->root = malloc(sizeof(BiNode));
  bi_node_init(bg_layer->root);
  BiNode* sky = make_sprite("assets/sky.png");
  bi_node_set_position(sky,context->w/2,context->h/2);
  bi_node_add_node(bg_layer->root,sky);
  bg_layer->textures[0] = sky->texture_mapping->texture;
  bg_layer->shader = bg_shader;

  // Foreground layer
  BiNode* root = make_sprite("assets/face01.png");
  bi_node_set_position(root,context->w/2,context->h/2);
  BiLayer *fg_layer = malloc(sizeof(BiLayer));
  bi_layer_init(fg_layer);
  fg_layer->root = root;
  bi_node_set_scale(root,2.0,2.0);
  fg_layer->textures[0] = root->texture_mapping->texture;
  fg_layer->shader = fg_shader;

  // Layer Group and Post Processing
  BiLayerGroup* layer_group = malloc(sizeof(BiLayerGroup));
  bi_layer_group_init(layer_group);
  //
  for(int i=0;i<2;i++){
    BiPostProcess* post_process = malloc(sizeof(BiPostProcess));
    bi_post_process_init(post_process);
    post_process->shader = pp_shaders[i];
    bi_layer_group_add_post_process(layer_group,post_process);
  }
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
