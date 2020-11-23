#include "common-core.h"

static bool on_click(BiNode* n, void *c, int x, int y, int button, bool pressed)
{
  if(pressed){
    printf("save...\n");
    BiContext *context = c;

    // !!!: retina display scaled x2!
    int w,h;
    SDL_GL_GetDrawableSize(context->window, &w, &h);

    int stride = w*4;
    uint8_t *data = malloc(w*h*32);

    glReadPixels(0,0,w,h, GL_RGBA, GL_UNSIGNED_BYTE, data);

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_LockSurface(surface);

    for(int i=0;i<h;i++){
      memcpy( surface->pixels+(h-1-i)*stride, data+i*stride, stride);
    }

    SDL_UnlockSurface(surface);

    IMG_SavePNG(surface,"capture.png");

    SDL_FreeSurface(surface);
    free(data);
    printf("done.\n");
  }
  return false;
}

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);
    bi_set_on_click(root, on_click, context);

    // texture node 1
    BiNode* sky = make_sprite("assets/sky.png");
    bi_node_set_position(sky,context->w/2,context->h/2);
    bi_add_node(root,sky);

    // texture node 2
    BiNode* face = make_sprite("assets/face01.png");
    bi_node_set_position(face,context->w/2,context->h/2);
    bi_add_node(root,face);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    bi_add_layer(context,layer);
    layer->root = root;
    layer->textures[1] = sky->texture_mapping->texture;
    layer->textures[0] = face->texture_mapping->texture;
}

int main(int argc, char* argv[])
{
    print_version();
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, true, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
