/* This shader draws a number surrounded by a circle
   Used for projector base
 */

varying vec2 vTexCoord;
uniform float number;

float width = 0.1;
float soft = 0.005;

vec4 ch_0 = vec4(0x007CC6,0xD6D6D6,0xD6D6C6,0x7C0000);
vec4 ch_1 = vec4(0x001030,0xF03030,0x303030,0xFC0000);
vec4 ch_2 = vec4(0x0078CC,0xCC0C18,0x3060CC,0xFC0000);
vec4 ch_3 = vec4(0x0078CC,0x0C0C38,0x0C0CCC,0x780000);
vec4 ch_4 = vec4(0x000C1C,0x3C6CCC,0xFE0C0C,0x1E0000);
vec4 ch_5 = vec4(0x00FCC0,0xC0C0F8,0x0C0CCC,0x780000);
vec4 ch_6 = vec4(0x003860,0xC0C0F8,0xCCCCCC,0x780000);
vec4 ch_7 = vec4(0x00FEC6,0xC6060C,0x183030,0x300000);
vec4 ch_8 = vec4(0x0078CC,0xCCEC78,0xDCCCCC,0x780000);
vec4 ch_9 = vec4(0x0078CC,0xCCCC7C,0x181830,0x700000);


//Extracts bit b from the given number.
//Shifts bits right (num / 2^bit) then ANDs the result with 1 (mod(result,2.0)).
float extract_bit(float n, float b)
{
  b = clamp(b,-1.0,24.0);
return floor(mod(floor(n / pow(2.0,floor(b))),2.0));
}

//Returns the pixel at uv in the given bit-packed sprite.
float sprite(vec4 spr, vec2 size, vec2 uv)
{
  uv = floor(uv);

  //Calculate the bit to extract (x + y * width) (flipped on x-axis)
  float bit = (size.x-uv.x-1.0) + uv.y * size.x;

  //Clipping bound to remove garbage outside the sprite's boundaries.
  bool bounds = all(greaterThanEqual(uv,vec2(0))) && all(lessThan(uv,size));

  float pixels = 0.0;
  pixels += extract_bit(spr.x, bit - 72.0);
  pixels += extract_bit(spr.y, bit - 48.0);
  pixels += extract_bit(spr.z, bit - 24.0);
  pixels += extract_bit(spr.w, bit - 00.0);

  return bounds ? pixels : 0.0;
}


#define CHAR_SIZE vec2(8, 12)
#define CHAR_SPACING vec2(8, 12)
#define MAX_INT_DIGITS 4

vec2 print_pos = vec2(0);


//Prints a character and moves the print position forward by 1 character width.
float print_char(vec4 ch, vec2 uv)
{
  float px = sprite(ch, CHAR_SIZE, uv - print_pos);
  print_pos.x += CHAR_SPACING.x;
  return px;
}

//Returns the digit sprite for the given number.
vec4 get_digit(float d)
{
  d = floor(d);

  if(d == 0.0) return ch_0;
  if(d == 1.0) return ch_1;
  if(d == 2.0) return ch_2;
  if(d == 3.0) return ch_3;
  if(d == 4.0) return ch_4;
  if(d == 5.0) return ch_5;
  if(d == 6.0) return ch_6;
  if(d == 7.0) return ch_7;
  if(d == 8.0) return ch_8;
  if(d == 9.0) return ch_9;
  return vec4(0.0);
}

float print_integer(float number, int zeros, vec2 uv)
{
  float result = 0.0;

  for(int i = MAX_INT_DIGITS;i >= 0;i--)
  {
      float digit = mod( number / pow(10.0, float(i)) , 10.0);

      if(abs(number) > pow(10.0, float(i)) || zeros > i  || i == 0) //Clip off leading zeros.
      {
        result += print_char(get_digit(digit),uv);
      }
  }
  return result;
}

float circle(vec2 pos, float r)
{
  float _distance = length(pos);
  float _v = (_distance ) / r;
  return 1.0 - smoothstep(1.0 - soft / r,1.0,_v);
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	  vec2 uv = vTexCoord;
    float outer_radius = 1.0 - soft;
    float v = circle(uv,outer_radius) - circle(uv,outer_radius - width);


    v += print_integer(number,2,uv * 15.0 + vec2(7.0,6.0));


    fragColor = vec4(v,v,v,v);
}
