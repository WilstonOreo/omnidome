#version 120

uniform float hours;
uniform float minutes;
uniform float seconds;
uniform float milliseconds;
uniform float t;
uniform bool blinking;
varying vec2 vTexCoord;

float segment(vec2 uv, bool On) {
	if (!On) return 0.0;

  float seg = (1.0-smoothstep(0.08,0.09+float(On)*0.02,abs(uv.x)))*

    (1.0-smoothstep(0.46,0.47+float(On)*0.02,abs(uv.y)+abs(uv.x)));

    seg *= (1.0-length(uv*vec2(3.8,0.9)));

  return seg;
}

float sevenSegment(vec2 uv,int num) {
    float seg= 0.0;
    seg += segment(uv.yx+vec2(-1.0, 0.0),num!=-1 && num!=1 && num!=4 );
    seg += segment(uv.xy+vec2(-0.5,-0.5),num!=-1 && num!=1 && num!=2 && num!=3 && num!=7);
    seg += segment(uv.xy+vec2( 0.5,-0.5),num!=-1 && num!=5 && num!=6 );
   	seg += segment(uv.yx+vec2( 0.0, 0.0),num!=-1 && num!=0 && num!=1 && num!=7 );
    seg += segment(uv.xy+vec2(-0.5, 0.5),num==0 || num==2 || num==6 || num==8);
	  seg += segment(uv.xy+vec2( 0.5, 0.5),num!=-1 && num!=2   );
    seg += segment(uv.yx+vec2( 1.0, 0.0),num!=-1 && num!=1 && num!=4 && num!=7  );
    return seg;
}

float showNum(vec2 uv,int nr) {
	float seg= 0.0;
	if (uv.x>0.0) { nr /= 10;
  seg += sevenSegment(uv+vec2(-0.75,0.0),nr);
	}
else seg += sevenSegment(uv+vec2( 0.75,0.0),int(mod(float(nr),10.0)));
    return seg;
}

void main() {
  vec2 uv = vTexCoord;
  uv -= vec2(0.5,0.5);
	uv *= vec2(-1.0,1.0);
  float val = 0.0;


  uv *= vec2(12.0,3.0);
  val += showNum(uv + vec2(-4.0,0.0),int(hours));
  val += showNum(uv + vec2(-0.0,0.0),int(minutes));
  val += showNum(uv + vec2(4.0,0.0),int(seconds));

	vec4 color = vec4(vec3(val),val*3.0);

	if (blinking) {
 		color *= cos(t*3.1416*16.0) > 0.0 ? vec4(0.0) : vec4(2.0,0.0,0.0,1.0);
	}

  gl_FragColor = color;
}
