#include "common-core.h"
#include "common-ext.h"

struct particle_data {
  float x;
  float y;
  float vx;
  float vy;
};

static void random_move(BiContext *c, void *userdata)
{
  BiNode *node = userdata;
  struct particle_data *p = (struct particle_data*)node->userdata;

  p->x += p->vx;
  p->y += p->vy;
  if(p->x < 0) p->x = 0;
  if(p->x > c->w) p->x = c->w;
  if(p->y < 0) p->y = 0;
  if(p->y > c->h) p->y = c->h;

  if( p->x == 0 || p->x == c->w) {
    p->vx = - p->vx;
  }
  if( p->y == 0 || p->y == c->h) {
    p->vy = - p->vy;
  }

  bi_node_set_position(node, p->x, p->y );
}

static BiNode* create_particle(BiContext* c,BiTexture *tex)
{
    BiNode* node = malloc(sizeof(BiNode));
    bi_node_init(node);

    // texture
    node->texture_mapping = malloc(sizeof(BiTextureMapping));
    bi_texture_mapping_init(node->texture_mapping);
    node->texture_mapping->texture = tex;
    bi_texture_mapping_set_bound(node->texture_mapping,0,0,tex->w,tex->h);
    node->anchor_x = node->anchor_y = 0.5;

    // position, scale, size
    bi_node_set_position(node, rand() % c->w, rand() % c->h );
    bi_node_set_size(node,tex->w,tex->h);
    node->scale_y = node->scale_x = 0.1 + rand()%200 / 100.0;

    // color
    bi_set_color(node->color, rand()%0xFF, rand()%0xFF, rand()%0xFF, 1+rand()%0xFE);

    // callback
    bi_set_on_update(node,random_move,NULL);

    // velocity
    struct particle_data* p = malloc(sizeof(struct particle_data));
    node->userdata = p;
    p->x = node->x;
    p->y = node->y;
    p->vx = (rand()%100/100.0) * 0.4 * (rand()%2==0 ? 1 : -1);
    p->vy = (rand()%100/100.0) * 0.4 * (rand()%2==0 ? 1 : -1);

    return node;
}

static void world_create(BiContext* context)
{
    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // particles
    BiTexture *ball_tex = malloc(sizeof(BiTexture));
    bi_texture_init(ball_tex);
    bi_texture_load_from_file(ball_tex,"assets/ball.png",false);
    // for(uint64_t i=0; i< 0xFFFE ; i++){
    // for(uint64_t i=0; i< 4096*4 ; i++){
    for(uint64_t i=0; i< 2048 ; i++){
      bi_add_node(root, create_particle(context,ball_tex) );
    }

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->root = root;
    bi_add_layer(context,layer);
    // addtive blending
    layer->blend_src = GL_SRC_ALPHA;
    layer->blend_dst = GL_ONE;
    // texture image
    layer->textures[0] = ball_tex;

    //
    // fps layer
    //
    BiFontAtlas *font = load_font();
    add_fps_layer(context,font);
}

int main(int argc, char* argv[])
{
    print_version();
    srand( bi_get_now() );
    BiContext* context = malloc(sizeof(BiContext));
    bi_init_context(context, 480, 320, 0, false, __FILE__);
    world_create(context);
    bi_start_run_loop(context);
    return 0;
}
