varying vec3 uvw_vertex_position;
varying vec3 uvw_normal;

const float PI = 3.14159265358979323846264;

uniform sampler2D texture; // Texture image

uniform mat4 matrix;

uniform bool flip_vertical;
uniform bool flip_horizontal;

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

float sphereIntersection(out vec3 uvw, in vec3 center) {
    vec3 n = uvw_normal;
    vec3 oc = normalize(uvw_vertex_position - center);
    float b = 2.0 * dot(n, oc);
    float c = dot(oc, oc) - 1.0;
    float disc = b * b - 4.0 * c;

    if (disc < 0.0)
        return -1.0;

    // compute q as described above
    float q;
    if (b < 0.0)
        q = (-b - sqrt(disc))/2.0;
    else
        q = (-b + sqrt(disc))/2.0;

    float t0 = q;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1) {
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < -0.0001)
        return -1.0;

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < -0.001) {
        uvw = oc + t1 * n;
    } else {
        uvw = oc + t0 * n;
    }

    return 1.0;
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

float intersection(out vec3 uvw);

float mapping(in vec3 uvw, out vec2 texCoords);

void main()
{
  vec3 uvw = (vec4(uvw_vertex_position,0.0) * matrix).xyz;
  if (intersection(uvw) < 0.0) {
//      discard;
  }

  uvw = (vec4(uvw,0.0) * matrix).xyz;

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
       color = texture2D(texture, texCoords).rgb;
    } else if (output_mode == 3) { // Output lighting only
        color = light_color(vec3(0.5));
    } else if (output_mode == 4) { // Output texture with lighting
        color = light_color(texture2D(texture,texCoords).rgb);
    }

    if (gray_output) {
      color = grayscale(color);
    }
    gl_FragColor = vec4(color,1.0);

  } else {
      discard;
  }
}
