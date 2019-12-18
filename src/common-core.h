#include <bi/util.h>
#include <bi/node.h>
#include <bi/context.h>
#include <bi/main_loop.h>
#include <bi/texture.h>
#include <bi/layer.h>
#include <bi/logger.h>

#include <stdlib.h>
#include <stdio.h>

static BiNode* make_sprite(const char* name)
{
  // load texture
  BiTexture *texture = malloc(sizeof(BiTexture));
  BiTextureImage *img = malloc(sizeof(BiTextureImage));
  if( bi_load_texture(name,img,false) ) {
    texture->texture_image = img;
    bi_set_texture_boundary(texture,0,0,texture->texture_image->w,texture->texture_image->h);
  }else{
    LOG("load error\n");
    return NULL;
  }

  BiNode* face = malloc(sizeof(BiNode));
  bi_node_init(face);
  face->w = texture->w;
  face->h = texture->h;
  face->anchor_x = 0.5;
  face->anchor_y = 0.5;
  face->texture = texture;
  bi_set_color( face->color, 0xFF, 0xFF, 0xFF, 0xFF);

  return face;
}

static BiNode* face_sprite() __attribute__((unused)) ;
static BiNode* face_sprite()
{
  return make_sprite("assets/face01.png");
}


static void print_stats(BiContext* context, void* userdata, double delta)
{
  double now = context->profile.frame_start_at;
  static double stats_shows_at = 0;
  if( context->debug && (now - stats_shows_at) > 1000 ) {
    LOG("FPS: %d/%.2f - frame: %.2f/%.2f [ms] - matrix_updated(AVG): %.2f - queued nodes: %d\n",
      context->profile.stats.actual_fps,
      context->profile.stats.estimated_fps,
      context->profile.stats.average_in_frame,
      context->profile.stats.average_frame_to_frame,
      context->profile.stats.total_matrix_updated / (double)(context->profile.stats.actual_fps),
      context->rendering_nodes_queue_size
    );
    stats_shows_at = now;
  }
}

static void __attribute__ ((unused)) enable_debug(BiContext *context)
{
  context->debug = true;
  context->on_update_callbacks[0].callback = print_stats;
  context->on_update_callbacks[0].userdata = NULL;
  context->on_update_callbacks_size = 1;
}


static void __attribute__ ((unused)) print_version()
{
#ifdef EMSCRIPTEN
  printf("emscripten %d.%d.%d\n", __EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__ );
#endif
  printf("clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__ );
  printf("GCC compatible %s\n", __VERSION__);
  printf("clang %s\n", __clang_version__);
  printf("bi-core %d.%d.%d\n", BI_CORE_MAJOR_VERSION, BI_CORE_MINOR_VERSION, BI_CORE_PATCHLEVEL);
  // printf("bi-ext %d.%d.%d\n", BI_EXT_MAJOR_VERSION,BI_EXT_MINOR_VERSION,BI_EXT_PATCHLEVEL);
  SDL_version compiled;
  SDL_version linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  printf("SDL(compile) %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
  printf("SDL(link) %d.%d.%d\n", linked.major, linked.minor, linked.patch);
}

static void __attribute__ ((unused)) print_gl_info()
{
  printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
  printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("Extensions: %s\n", glGetString(GL_EXTENSIONS));

  // iphone7: 4096
  GLint max_texture_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
  printf("GL_MAX_TEXTURE_SIZE: %d\n",max_texture_size);

  // iphone7: 8
  GLint max_texture_image_units;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
  printf("GL_MAX_TEXTURE_IMAGE_UNITS: %d\n",max_texture_image_units);
}
