#include "common-core.h"
#include "common-ext.h"

struct particle_data {
  float x;
  float y;
  float vx;
  float vy;
};

static void random_move(BiNode* node, void *context, void *callback_context, double delta)
{
  struct particle_data *p = (struct particle_data*)node->userdata;
  BiContext* c = (BiContext*)context;

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

static BiNode* create_particle(BiContext* c,BiTextureImage *img)
{
    BiNode* node = malloc(sizeof(BiNode));
    bi_node_init(node);

    // texture
    node->texture = malloc(sizeof(BiTexture));
    node->texture->texture_image = img;
    node->anchor_x = node->anchor_y = 0.5;
    bi_set_texture_boundary(node->texture,0,0,img->w,img->h);

    // position, scale, size
    bi_node_set_position(node, rand() % c->w, rand() % c->h );
    bi_node_set_size(node,img->w,img->h);
    node->scale_y = node->scale_x = rand()%200 / 100.0;

    // color
    bi_set_color(node->color, rand()%0xFF, rand()%0xFF, rand()%0xFF, rand()%0xFF);

    // callback
    bi_set_on_update(node,random_move,NULL);

    // velocity
    struct particle_data* p = malloc(sizeof(struct particle_data));
    node->userdata = p;
    p->x = node->x;
    p->y = node->y;
    p->vx = (rand()%100/100.0) * 4 * (rand()%2==0 ? 1 : -1);
    p->vy = (rand()%100/100.0) * 4 * (rand()%2==0 ? 1 : -1);

    return node;
}

static void world_create(BiContext* context)
{
    context->debug = true;

    // root node
    BiNode* root = malloc(sizeof(BiNode));
    bi_node_init(root);

    // particles
    BiTextureImage *ball_img = malloc(sizeof(BiTextureImage));
    bi_load_texture("assets/ball.png",ball_img,false,1);
    for(int i=0; i<4096; i++){
      bi_add_node(root, create_particle(context,ball_img) );
    }

    // layer
    BiLayer *layer = malloc(sizeof(BiLayer));
    bi_layer_init(layer);
    layer->root = root;
    bi_add_layer(context,layer);
    // addtive blending
    layer->blend_src = GL_SRC_ALPHA;
    layer->blend_dst = GL_ONE;

    //
    // fps layer
    //
    add_fps_layer(context);
}

int main(int argc, char* argv[])
{
    srand( bi_get_now() );
    BiContext _context;
    BiContext* context = &_context;
    bi_init_context(context, 480, 320, 0, __FILE__, world_create);
    bi_start_run_loop(context);
    return 0;
}
