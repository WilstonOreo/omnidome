#version 120

uniform vec2 ruler_pos;
uniform bool flip_text;
uniform bool display_numbers;
varying vec2 vTexCoord;

#include ":/shaders/digits.h"

float text_front(vec2 pos)
{
	pos *= CHAR_SIZE*10.0;
	pos.y -= 53.5;
  pos.x -= 20.0;
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
	pos *= CHAR_SIZE*10.0;
	pos.y -= 53.5;
  pos.x -= 24.5;
	float col = 0.0;
	col += print_char(ch_B,pos);
	col += print_char(ch_A,pos);
	col += print_char(ch_C,pos);
	col += print_char(ch_K,pos);
	return col;
}

float text_left(vec2 pos)
{
	pos *= CHAR_SIZE*10.0;
	pos.y -= 53.5;
  pos.x -= 24.5;
	float col = 0.0;
	col += print_char(ch_L,pos);
	col += print_char(ch_E,pos);
	col += print_char(ch_F,pos);
	col += print_char(ch_T,pos);
	return col;
}


float text_right(vec2 pos)
{
	pos *= CHAR_SIZE*10.0;
	pos.y -= 53.5;
  pos.x -= 20.0;
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
	pos *= CHAR_SIZE*10.0;
	pos.y -= 53.5;
  pos.x -= 29.0;
	float col = 0.0;
	col += print_char(ch_T,pos);
	col += print_char(ch_O,pos);
	col += print_char(ch_P,pos);
	return col;
}


float text_bottom(vec2 pos)
{
	pos *= CHAR_SIZE*10.0;
	pos.y -= 53.5;
  pos.x -= 15.5;
	float col = 0.0;
	col += print_char(ch_B,pos);
	col += print_char(ch_O,pos);
	col += print_char(ch_T,pos);
	col += print_char(ch_T,pos);
	col += print_char(ch_O,pos);
    col += print_char(ch_M,pos);
    return col;
}

float digit(vec2 pos, float number) {
	pos *= CHAR_SIZE;
  return print_char(get_digit(number),pos,0.0);
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
    uv = fract(uv);
    vec3 result_color = vec3(0.0);
    vec2 grid_st = uv*100.;
    result_color += 1.5*color*grid(grid_st,1.0/100.0,1.0);
    result_color += 0.8*color*grid(grid_st,1.0/50.0,0.4);
    result_color += 0.6*color*grid(grid_st,1.0/10.0,0.4);
    result_color += 0.4*color*grid(grid_st,1.0/2.0,0.3);

    // Crosses
    vec2 crosses_st = uv + .5;
    crosses_st *= 1.;
    vec2 crosses_st_f = fract(crosses_st);
    result_color  *= 1.-cross(crosses_st_f,vec2(.3,.3));
    result_color += vec3(.9)*cross(crosses_st_f,vec2(.2,.2));

    if (display_numbers) {
      vec2 number_coords = fract(uv*10.0);
      vec2 numbers = floor(uv*10.0);
      result_color += vec3(digit(number_coords*4.0 - vec2(1.125,1.5),numbers.x));
      result_color += vec3(digit(number_coords*4.0 - vec2(2.125,1.5),numbers.y));
    }

    return result_color;
}


void main(void)
{
	vec2 uv = vTexCoord;
    vec3 color = vec3(0.0);

    uv *= vec2(6.0,1.0);

    float t = floor(uv.x);
    vec2 st = fract(uv * (flip_text ? vec2(-1.0,1.0) : vec2(1.0)));

    if (t == 0.0) {
    	color += cubeSide(vec3(1.0,0.0,0.0),uv);
      print_pos.x = 0.0;
        color += vec3(text_front(st));
    } else
    if (t == 1.0) {
    	color += cubeSide(vec3(0.0,1.0,1.0),uv);
      print_pos.x = 0.0;
        color += vec3(text_back(st));
    } else
    if (t == 2.0) {
    	color += cubeSide(vec3(0.0,1.0,0.0),uv);
      print_pos.x = 0.0;
        color += vec3(text_left(st));
    } else
    if (t == 3.0) {
    	color += cubeSide(vec3(1.0,0.0,1.0),uv);
      print_pos.x = 0.0;
        color += vec3(text_right(st));
    } else
    if (t == 4.0) {
    	color += cubeSide(vec3(0.0,0.0,1.0),uv);
      print_pos.x = 0.0;
        color += vec3(text_top(st));
    } else
    if (t == 5.0) {
    	color += cubeSide(vec3(1.0,1.0,0.0),uv);
      print_pos.x = 0.0;
        color += vec3(text_bottom(st));
    }
    gl_FragColor = vec4(color,1.0);
}
