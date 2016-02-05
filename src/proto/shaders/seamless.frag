uniform sampler2D texture;
uniform float factor;
uniform float offset;
uniform float time;
uniform float gamma;

void main()
{
  vec2 uv = gl_TexCoord[0].st;
  //uv.x += 0.5;
//  uv.x *= 0.5;

  //uv.x =  factor + uv.x * (factor);

  float f1 = abs((factor - uv.x)) / factor  ;// cos((uv.x)*3.1415/2.0);
  if (uv.x > factor ) f1 = 0.0;
  
  float f2 = abs((1.0 - factor - uv.x)) / factor;// cos((uv.x)*3.1415/2.0);
  if (uv.x < 0.5 - factor ) f2 = 0.0;
  
  f1 = clamp(pow(f1,gamma),0.0,1.0);
  f2 = clamp(pow(f2,gamma),0.0,1.0);

  gl_FragColor = texture2D(texture,uv) *(1.0 - f1) * (1.0 - f2);
  gl_FragColor += 
    texture2D(texture,vec2(uv.x,uv.y)) * f1 + 
    texture2D(texture,vec2(-uv.x,uv.y))*f2; // + uv.x * texture2D(texture,uv);
 // gl_FragColor += texture2D(texture,vec2(1.0 - uv.x,uv.y)) *f; // + uv.x * texture2D(texture,uv);
//  gl_FragColor += texture2D(texture,vec2(1.0 - uv.x,uv.y)) *f; // + uv.x * texture2D(texture,uv);
  
  
  //gl_FragColor += texture2D(texture,vec2(uv.x,uv.y));

}

