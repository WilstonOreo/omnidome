
uniform vec2 ruler_pos;
varying vec2 vTexCoord;

vec4 ch_A = vec4(0x003078,0xCCCCCC,0xFCCCCC,0xCC0000);
vec4 ch_B = vec4(0x00FC66,0x66667C,0x666666,0xFC0000);
vec4 ch_C = vec4(0x003C66,0xC6C0C0,0xC0C666,0x3C0000);
vec4 ch_D = vec4(0x00F86C,0x666666,0x66666C,0xF80000);
vec4 ch_E = vec4(0x00FE62,0x60647C,0x646062,0xFE0000);
vec4 ch_F = vec4(0x00FE66,0x62647C,0x646060,0xF00000);
vec4 ch_G = vec4(0x003C66,0xC6C0C0,0xCEC666,0x3E0000);
vec4 ch_H = vec4(0x00CCCC,0xCCCCFC,0xCCCCCC,0xCC0000);
vec4 ch_I = vec4(0x007830,0x303030,0x303030,0x780000);
vec4 ch_J = vec4(0x001E0C,0x0C0C0C,0xCCCCCC,0x780000);
vec4 ch_K = vec4(0x00E666,0x6C6C78,0x6C6C66,0xE60000);
vec4 ch_L = vec4(0x00F060,0x606060,0x626666,0xFE0000);
vec4 ch_M = vec4(0x00C6EE,0xFEFED6,0xC6C6C6,0xC60000);
vec4 ch_N = vec4(0x00C6C6,0xE6F6FE,0xDECEC6,0xC60000);
vec4 ch_O = vec4(0x00386C,0xC6C6C6,0xC6C66C,0x380000);
vec4 ch_P = vec4(0x00FC66,0x66667C,0x606060,0xF00000);
vec4 ch_Q = vec4(0x00386C,0xC6C6C6,0xCEDE7C,0x0C1E00);
vec4 ch_R = vec4(0x00FC66,0x66667C,0x6C6666,0xE60000);
vec4 ch_S = vec4(0x0078CC,0xCCC070,0x18CCCC,0x780000);
vec4 ch_T = vec4(0x00FCB4,0x303030,0x303030,0x780000);
vec4 ch_U = vec4(0x00CCCC,0xCCCCCC,0xCCCCCC,0x780000);
vec4 ch_V = vec4(0x00CCCC,0xCCCCCC,0xCCCC78,0x300000);
vec4 ch_W = vec4(0x00C6C6,0xC6C6D6,0xD66C6C,0x6C0000);
vec4 ch_X = vec4(0x00CCCC,0xCC7830,0x78CCCC,0xCC0000);
vec4 ch_Y = vec4(0x00CCCC,0xCCCC78,0x303030,0x780000);
vec4 ch_Z = vec4(0x00FECE,0x981830,0x6062C6,0xFE0000);
vec4 ch_a = vec4(0x000000,0x00780C,0x7CCCCC,0x760000);
vec4 ch_b = vec4(0x00E060,0x607C66,0x666666,0xDC0000);
vec4 ch_c = vec4(0x000000,0x0078CC,0xC0C0CC,0x780000);
vec4 ch_d = vec4(0x001C0C,0x0C7CCC,0xCCCCCC,0x760000);
vec4 ch_e = vec4(0x000000,0x0078CC,0xFCC0CC,0x780000);
vec4 ch_f = vec4(0x00386C,0x6060F8,0x606060,0xF00000);
vec4 ch_g = vec4(0x000000,0x0076CC,0xCCCC7C,0x0CCC78);
vec4 ch_h = vec4(0x00E060,0x606C76,0x666666,0xE60000);
vec4 ch_i = vec4(0x001818,0x007818,0x181818,0x7E0000);
vec4 ch_j = vec4(0x000C0C,0x003C0C,0x0C0C0C,0xCCCC78);
vec4 ch_k = vec4(0x00E060,0x60666C,0x786C66,0xE60000);
vec4 ch_l = vec4(0x007818,0x181818,0x181818,0x7E0000);
vec4 ch_m = vec4(0x000000,0x00FCD6,0xD6D6D6,0xC60000);
vec4 ch_n = vec4(0x000000,0x00F8CC,0xCCCCCC,0xCC0000);
vec4 ch_o = vec4(0x000000,0x0078CC,0xCCCCCC,0x780000);
vec4 ch_p = vec4(0x000000,0x00DC66,0x666666,0x7C60F0);
vec4 ch_q = vec4(0x000000,0x0076CC,0xCCCCCC,0x7C0C1E);
vec4 ch_r = vec4(0x000000,0x00EC6E,0x766060,0xF00000);
vec4 ch_s = vec4(0x000000,0x0078CC,0x6018CC,0x780000);
vec4 ch_t = vec4(0x000020,0x60FC60,0x60606C,0x380000);
vec4 ch_u = vec4(0x000000,0x00CCCC,0xCCCCCC,0x760000);
vec4 ch_v = vec4(0x000000,0x00CCCC,0xCCCC78,0x300000);
vec4 ch_w = vec4(0x000000,0x00C6C6,0xD6D66C,0x6C0000);
vec4 ch_x = vec4(0x000000,0x00C66C,0x38386C,0xC60000);
vec4 ch_y = vec4(0x000000,0x006666,0x66663C,0x0C18F0);
vec4 ch_z = vec4(0x000000,0x00FC8C,0x1860C4,0xFC0000);


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


#define CHAR_SIZE vec2(8.0, 12.0)
#define CHAR_SPACING vec2(8.0, 12.0)
#define MAX_INT_DIGITS 4

vec2 print_pos = vec2(0.0);


//Prints a character and moves the print position forward by 1 character width.
float print_char(vec4 ch, vec2 uv)
{
  float px = sprite(ch, CHAR_SIZE, uv - print_pos);
  print_pos.x += CHAR_SPACING.x;
  return px;
}

float text_front(vec2 pos)
{
	pos *= CHAR_SIZE*8.0;
	pos.y -= 42.0;
    pos.x -= 13.0;
	float col = 0.0;
	col += print_char(ch_F,pos);
	col += print_char(ch_R,pos);
	col += print_char(ch_O,pos);
	col += print_char(ch_N,pos);
	col += print_char(ch_T,pos);
	return col;
}

float text_back(vec2 pos)
{
	pos *= CHAR_SIZE*8.0;
	pos.y -= 42.0;
    pos.x -= 16.0;
	float col = 0.0;
	col += print_char(ch_B,pos);
	col += print_char(ch_A,pos);
	col += print_char(ch_C,pos);
	col += print_char(ch_K,pos);
	return col;
}

float text_left(vec2 pos)
{
	pos *= CHAR_SIZE*8.0;
	pos.y -= 42.0;
    pos.x -= 16.0;
	float col = 0.0;
	col += print_char(ch_L,pos);
	col += print_char(ch_E,pos);
	col += print_char(ch_F,pos);
	col += print_char(ch_T,pos);
	return col;
}


float text_right(vec2 pos)
{
	pos *= CHAR_SIZE*8.0;
	pos.y -= 42.0;
    pos.x -= 13.0;
	float col = 0.0;
	col += print_char(ch_R,pos);
	col += print_char(ch_I,pos);
	col += print_char(ch_G,pos);
	col += print_char(ch_H,pos);
	col += print_char(ch_T,pos);
    return col;
}

float text_top(vec2 pos)
{
	pos *= CHAR_SIZE*8.0;
	pos.y -= 42.0;
    pos.x -= 19.0;
	float col = 0.0;
	col += print_char(ch_T,pos);
	col += print_char(ch_O,pos);
	col += print_char(ch_P,pos);
	return col;
}


float text_bottom(vec2 pos)
{
	pos *= CHAR_SIZE*8.0;
	pos.y -= 42.0;
    pos.x -= 10.0;
	float col = 0.0;
	col += print_char(ch_B,pos);
	col += print_char(ch_O,pos);
	col += print_char(ch_T,pos);
	col += print_char(ch_T,pos);
	col += print_char(ch_O,pos);
    col += print_char(ch_M,pos);
    return col;
}



float box(in vec2 st, in vec2 size){
    size = vec2(0.5) - size*0.5;
    vec2 uv = smoothstep(size,
                        size+vec2(0.001),
                        st);
    uv *= smoothstep(size,
                    size+vec2(0.001),
                    vec2(1.0)-st);
    return uv.x*uv.y;
}

float grid(vec2 st, float res, float thickness){
    vec2 grid =  fract(st*res + 0.5*thickness*res);
    return 1.-(step(res*thickness,grid.x) * step(res*thickness,grid.y));
}

float cross(in vec2 st, vec2 size){
    return  clamp(box(st, vec2(size.x*0.5,size.y*0.125)) +
            box(st, vec2(size.y*0.125,size.x*0.5)),0.,1.);
}

vec3 cubeSide(in vec3 color, vec2 uv) {
    // Grid
    vec3 result_color = vec3(0.0);
    vec2 grid_st = uv*100.;
    result_color += 1.0*color*grid(grid_st,1.0/100.0,1.0);
    result_color += 0.5*color*grid(grid_st,1.0/50.0,0.4);
    result_color += 0.2*color*grid(grid_st,1.0/10.0,0.4);
    result_color += 0.1*color*grid(grid_st,1.0/2.0,0.3);

    // Crosses
    vec2 crosses_st = uv + .5;
    crosses_st *= 1.;
    vec2 crosses_st_f = fract(crosses_st);
    result_color  *= 1.-cross(crosses_st_f,vec2(.3,.3));
    result_color += vec3(.9)*cross(crosses_st_f,vec2(.2,.2));

    return result_color;
}


void main(void)
{
	vec2 uv = vTexCoord;
    vec3 color = vec3(0.0);

    uv *= vec2(6.0,1.0);

    float t = floor(uv.x);
    vec2 st = fract(uv);

    if (t == 0.0) {
    	color += cubeSide(vec3(1.0,0.0,0.0),uv);
        color += vec3(text_front(st));
    } else
    if (t == 1.0) {
    	color += cubeSide(vec3(0.0,1.0,1.0),uv);
        color += vec3(text_back(st));
    } else
    if (t == 2.0) {
    	color += cubeSide(vec3(0.0,1.0,0.0),uv);
        color += vec3(text_left(st));
    } else
    if (t == 3.0) {
    	color += cubeSide(vec3(1.0,0.0,1.0),uv);
        color += vec3(text_right(fract(uv)));
    } else
    if (t == 4.0) {
    	color += cubeSide(vec3(0.0,0.0,1.0),uv);
        color += vec3(text_top(st));

    } else
    if (t == 5.0) {
    	color += cubeSide(vec3(1.0,1.0,0.0),uv);
        color += vec3(text_bottom(st));
    }

    gl_FragColor = vec4(color,1.0);
}
