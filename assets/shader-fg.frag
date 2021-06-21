
varying vec3 uv;
varying vec4 color;
uniform sampler2D sampler[8];
uniform float time;
uniform vec2 resolution;
uniform vec4 optional_attributes;

vec4 getTextureColor(int samplerID,vec2 xy) {
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

#define GRID 10.0
#define SIZE 6.0

void main()
{
  vec2 xy = gl_FragCoord.xy;
  if( mod(xy.x,GRID) <= SIZE && mod(xy.y,GRID) <= SIZE ) {
    gl_FragColor = getTextureColor(int(uv.z), uv.xy) * color;
  }else{
    gl_FragColor = vec4(0.0);
  }
}
