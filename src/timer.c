#include "common-core.h"
#include "common-ext.h"

static bool rotate_face(int64_t now,BiTimer* timer)
{
  BiNode *node = timer->userdata;
  // LOG("rotate_face %lld\n", now);
  bi_node_set_angle(node, node->angle + 1 * M_PI/180);
  return true;
}

struct global_timer_container {
  BiNode *node;
  BiContext *context;
};

static bool global_timer_funciton(int64_t now,BiTimer* timer)
{
  static int counter = 0;
  LOG("this is global timer function: %ld\n", (long)now);
  struct global_timer_container *container = timer->userdata;
  BiContext *context = container->context;
  counter++;
  if(counter>2){
    LOG("timer sucide\n");
    bi_remove_timer(&context->timers,timer);
    free(container);
    free(timer);
  }
  return true;
}

static bool global_timer_funciton2(int64_t now,BiTimer* timer)
{
  struct global_timer_container *container = timer->userdata;
  BiNode *node = container->node;
  bi_set_color( node->color, rand()%0xFF, rand()%0xFF, rand()%0xFF, 0xFF);
  LOG("change color: %ld\n", (long)now);
  return true;
}

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // sprite node
    BiNode* face = face_sprite();
    face->x = context->w/2;
    face->y = context->h/2;
    bi_set_color( face->color, 0xFF, 0xFF, 0xFF, 0xFF);
    // add to scene graph
    bi_node_add_node(root,face);

    // add global timer
    struct global_timer_container *container = malloc(sizeof(struct global_timer_container));
    container->node = face;
    container->context = context;
    BiTimer *global_timer = malloc(sizeof(BiTimer));
    bi_timer_init(global_timer, global_timer_funciton, 1000, -1, container);
    bi_add_timer(&context->timers,global_timer);

    // add global timer2
    struct global_timer_container *container2 = malloc(sizeof(struct global_timer_container));
    container2->node = face;
    container2->context = context;
    BiTimer *global_timer2 = malloc(sizeof(BiTimer));
    bi_timer_init(global_timer2, global_timer_funciton2, 1000, -1, container2);
    bi_add_timer(&context->timers,global_timer2);

    // add timer
    BiTimer *timer = malloc(sizeof(BiTimer));
    bi_timer_init(timer, rotate_face, 10, -1, face);
    bi_add_timer(&face->timers,timer);

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->textures[0] = face->texture_mapping->texture;
    bi_add_layer(context,layer);
    layer->root = root;

    // fps layer
    BiFontAtlas *font = load_font();
    add_fps_layer(context,font);
}

int main(int argc,char* argv[])
{
  print_version();
  BiContext* context = malloc(sizeof(BiContext));
  bi_init_context(context, 480, 320, 0, false, __FILE__);
  world_create(context);
  bi_start_run_loop(context);
  return 0;
}
