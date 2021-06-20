
#ifdef __EMSCRIPTEN__
#define SHADER_VERSION "#version 100\n"
#define FRAGMENT_SHADER_HEADER "precision highp float;\n"
#else
#define SHADER_VERSION "#version 120\n"
#define FRAGMENT_SHADER_HEADER ""
#endif

#define D(...) #__VA_ARGS__ "\n";

const char *PIXELATE_FRAGMENT_SHADER = SHADER_VERSION FRAGMENT_SHADER_HEADER D(
varying vec3 uv;
varying vec4 color;
uniform sampler2D sampler[8];
uniform float time;
uniform vec2 resolution;
uniform vec4 optional_attributes;

vec4 getTextureColor(int samplerID,vec2 xy) {
  // WebGL not supported dynamic indexing for sampler...
  if(samplerID==0){ return texture2D(sampler[0], xy); }
  if(samplerID==1){ return texture2D(sampler[1], xy); }
  if(samplerID==2){ return texture2D(sampler[2], xy); }
  if(samplerID==3){ return texture2D(sampler[3], xy); }
  if(samplerID==4){ return texture2D(sampler[4], xy); }
  if(samplerID==5){ return texture2D(sampler[5], xy); }
  if(samplerID==6){ return texture2D(sampler[6], xy); }
  if(samplerID==7){ return texture2D(sampler[7], xy); }
  return vec4(0);
}

void main()
{
  float pixel_size = 8.0;
  float unit_x = resolution.x / pixel_size;
  float unit_y = resolution.y / pixel_size;
  float uv_x = floor(uv.x * unit_x) / unit_x;
  float uv_y = floor(uv.y * unit_y) / unit_y;

  int samplerID = int(uv.z);
  if( 0 <= samplerID && samplerID <= 7 ) {
    gl_FragColor = getTextureColor(samplerID, vec2(uv_x, uv_y)) * color;
  }else{
    gl_FragColor = color;
  }
}
);

#undef D
