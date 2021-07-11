#include <bi/util.h>
#include <bi/node.h>
#include <bi/context.h>
#include <bi/main_loop.h>
#include <bi/texture.h>
#include <bi/layer.h>
#include <bi/logger.h>

#include <stdlib.h>
#include <stdio.h>

static BiTextureMapping* make_texture_mapping(const char* name)
{
  // load texture
  BiTextureMapping *mapping = malloc(sizeof(BiTextureMapping));
  bi_texture_mapping_init(mapping);
  BiTexture *texture = malloc(sizeof(BiTexture));
  if( bi_texture_init_with_filename(texture,name,false) ) {
    mapping->texture = texture;
    bi_texture_mapping_set_bound(mapping,0,0,texture->w,texture->h);
  }else{
    LOG("load error\n");
    return NULL;
  }
  return mapping;
}

static BiNode* make_sprite_from_mapping(BiTextureMapping *mapping)
{
  BiNode* sprite = malloc(sizeof(BiNode));
  bi_node_init(sprite);
  sprite->w = mapping->texture->w;
  sprite->h = mapping->texture->h;
  sprite->anchor_x = 0.5;
  sprite->anchor_y = 0.5;
  sprite->texture_mapping = mapping;
  bi_set_color( sprite->color, 0xFF, 0xFF, 0xFF, 0xFF);
  return sprite;
}

static BiNode* make_sprite(const char* name)
{
  // load texture
  BiTextureMapping *mapping = make_texture_mapping(name);
  return make_sprite_from_mapping(mapping);
}

static BiNode* face_sprite() __attribute__((unused)) ;
static BiNode* face_sprite()
{
  return make_sprite("assets/face01.png");
}

static void __attribute__ ((unused)) print_version()
{
#ifdef EMSCRIPTEN
  printf("emscripten %d.%d.%d\n", __EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__ );
#endif
#ifdef __clang_version__
  printf("clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__ );
  printf("clang %s\n", __clang_version__);
#endif
  printf("GCC compatible %s\n", __VERSION__);
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
