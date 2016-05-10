varying vec3 uvw_vertex_position;
varying vec3 uvw_normal;

const float PI = 3.14159265358979323846264;

uniform sampler2DRect texture; // Texture image
uniform vec2 texture_size; // Texture image

uniform mat4 matrix;
uniform vec3 offset;
uniform bool flip_vertical;
uniform bool flip_horizontal;
uniform float transparency;

/// Select whether output texture, texture coordintates
/// or uvw coordinates
/// 0: Output texture
/// 1: Output texture coordinates
/// 2: Output uvw coordinates
uniform int output_mode;

uniform bool gray_output;

vec3 grayscale(vec3 c)
{
  float v = c.r*0.299 + c.g*0.587 + c.b*0.114;
  return vec3(v);
}

#define MAX_LIGHTS 3

vec3 light_color(vec3 color)
{
   vec3 N = uvw_normal;
   vec3 v = uvw_vertex_position;
   vec3 finalColor = vec3(0.0, 0.0, 0.0);

   for (int i=0;i<MAX_LIGHTS;i++)
   {
      vec3 L = normalize(gl_LightSource[i].position.xyz - v);
      vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
      vec3 R = normalize(-reflect(L,N));

      //calculate Ambient Term:
      vec3 Iamb = vec3(0.0);

      //calculate Diffuse Term:
      vec3 Idiff = vec3(max(dot(N,L), 0.0));
        Idiff = clamp(Idiff, 0.0, 1.0);

      // calculate Specular Term:
      vec3 Ispec = vec3(1.0)
             * pow(max(dot(R,E),0.0),gl_FrontMaterial.shininess);
      Ispec = clamp(Ispec, 0.0, 1.0);

      finalColor += Iamb + Idiff + Ispec;
   }

   return color*finalColor;
}

float mapping(in vec3 uvw, out vec2 texCoords);

void main()
{
  vec3 uvw = ((vec4(uvw_vertex_position - offset,1.0) * matrix).xyz);

  if (output_mode == 2) // Output UVW map only
  {
    gl_FragColor = vec4(0.5*(normalize(uvw.xyz) + vec3(1.0)),1.0);
    return;
  }


  vec2 texCoords;
  if (mapping(uvw,texCoords) > 0.0)
  {
    if (flip_horizontal) texCoords.x = 1.0 - texCoords.x;
    if (flip_vertical) texCoords.y = 1.0 - texCoords.y;

    if (output_mode == 1) // Output texture coordinates
    {
      gl_FragColor = vec4(texCoords.xy,0.0,1.0);
      return;
    }
    vec3 color;

    if (output_mode == 0) {
       color = texture2DRect(texture, texCoords * texture_size).rgb;
    } else if (output_mode == 3) { // Output lighting only
        color = light_color(vec3(0.5));
    } else if (output_mode == 4) { // Output texture with lighting
        color = light_color(texture2DRect(texture,texCoords * texture_size).rgb);
    }

    if (gray_output) {
      color = grayscale(color);
    }
    gl_FragColor = vec4(color,transparency);

  } else {
      discard;
  }
}
