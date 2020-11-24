
#ifdef __EMSCRIPTEN__
#define SHADER_VERSION "#version 100\n"
#define FRAGMENT_SHADER_HEADER "precision highp float;\n"
#else
#define SHADER_VERSION "#version 120\n"
#define FRAGMENT_SHADER_HEADER ""
#endif

#define D(...) #__VA_ARGS__ "\n";

const char *WAVE_FRAGMENT_SHADER = SHADER_VERSION FRAGMENT_SHADER_HEADER D(
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
  int samplerID = int(uv.z);
  if( 0 <= samplerID && samplerID <= 7 ) {
    float offset = cos(gl_FragCoord.y * 0.1 + time) * sin(time) * 0.02;
    gl_FragColor = getTextureColor(samplerID, vec2(uv.x+offset, uv.y)) * color;
  }else{
    gl_FragColor = color;
  }
}
);

#undef D
