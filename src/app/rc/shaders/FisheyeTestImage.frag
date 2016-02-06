
varying vec2 vTexCoord;
uniform vec2 ruler_pos;

const float PI = 3.14159265358979323846264;


// Smooth HSV to RGB conversion
vec3 hsv2rgb_smooth( in float c )
{
  vec3 rgb = clamp( abs(mod(c*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
	rgb = rgb*rgb*(3.0-2.0*rgb); // cubic smoothing
	return rgb;
}

float spherical_direction(in vec2 uv, out vec3 rd)
{
  float theta = (uv.t) * PI,
        phi = (uv.s - 0.5)* 2.0 * PI;
  rd = vec3(sin(theta) * sin(phi), sin(theta) * cos(phi), cos(theta));
  return 1.0;
}

float dist_vertical_line( vec3 p)
{
  return length(vec2(length(p.xz)-1.0,p.y));
}

float dist_horizontal_line( vec3 p)
{
  return length(vec2(length(p.xy)-1.0,p.z));
}

vec2 pos;
float sin_theta, cos_theta;
float sin_phi, cos_phi;

vec2 linePosition(in vec2 numLines)
{
  return vec2(
    mod(pos.x*numLines.x + 0.25,1.0),
    mod(pos.y*numLines.y + 0.5,1.0));
}

float grid(in vec2 numLines, in float thickness)
{
  vec2 linePos = linePosition(numLines);
  vec2 vertical = vec2(linePos.x,pos.y);
  vec2 horizontal = vec2(pos.x,linePos.y);

  float theta = linePos.y * PI;
  float phi = (linePos.x - 0.5)* 2.0 * PI;
  vec3 vertPoint = vec3(sin_theta * sin(phi), sin_theta * cos(phi), cos_theta);
  vec3 horzPoint = vec3(sin(theta) * sin_phi, sin(theta) * cos_phi, cos(theta));

  return min(dist_vertical_line(vertPoint),dist_horizontal_line(horzPoint)) / thickness;
}


float ruler()
{
  float theta = (pos.t) * PI,
        phi = (pos.s - ruler_pos.x + 0.25)* 2.0 * PI;
  vec3 rd = vec3(sin(theta) * sin(phi), sin(theta) * cos(phi), cos(theta));

  float mouse_theta = (0.5 + ruler_pos.y ) * PI;
  float mouse_phi = (ruler_pos.x - 0.5)* 2.0 * PI;
  vec3 mousePoint = vec3(sin(mouse_theta) * sin(mouse_phi), sin(mouse_theta) * cos(mouse_phi), cos(mouse_theta));

  float horizontal_line = length(vec2(length(rd.xy) - length(mousePoint.xy),rd.z  + mousePoint.z));

  float vertical_line = length(vec2(length(rd.xz)-1.0,rd.y));

  return min(vertical_line,horizontal_line) / 0.01;
}


float segment(vec2 uv, bool On) {
	return (On) ?  (1.0 - smoothstep(0.05,0.15,abs(uv.x))) *
			       (1.-smoothstep(0.35,0.55,abs(uv.y)+abs(uv.x)))
		        : 0.;
}

float digit(vec2 uv,int num) {
	float seg= 0.;
    seg += segment(uv.yx+vec2(-1., 0.),num!=-1 && num!=1 && num!=4                    );
	seg += segment(uv.xy+vec2(-.5,-.5),num!=-1 && num!=1 && num!=2 && num!=3 && num!=7);
	seg += segment(uv.xy+vec2( .5,-.5),num!=-1 && num!=5 && num!=6                    );
   	seg += segment(uv.yx+vec2( 0., 0.),num!=-1 && num!=0 && num!=1 && num!=7          );
	seg += segment(uv.xy+vec2(-.5, .5),num==0 || num==2 || num==6 || num==8           );
	seg += segment(uv.xy+vec2( .5, .5),num!=-1 && num!=2                              );
    seg += segment(uv.yx+vec2( 1., 0.),num!=-1 && num!=1 && num!=4 && num!=7          );
	return seg;
}

float showNum(vec2 uv,int nr, bool zeroTrim) { // nr: 2 digits + sgn . zeroTrim: trim leading "0"
	if (abs(uv.x)>2.*1.5 || abs(uv.y)>1.2) return 0.;

	if (nr<0) {
		nr = -nr;
		if (uv.x>1.5) {
			uv.x -= 2.;
			return segment(uv.yx,true); // minus sign.
		}
	}

	if (uv.x>0.) {
		nr /= 10; if (nr==0 && zeroTrim) nr = -1;
		uv -= vec2(.75,0.);
	} else {
		uv += vec2(.75,0.);
		nr = int(mod(float(nr),10.));
	}

	return digit(uv,nr);
}

//    ... end of digits adapted from Andre

#define STEPX .875
#define STEPY 1.5

// 2digit int + sign
float display(vec2 uv, vec2 pos, vec2 scale, int number) { // dot: draw separator
	uv = (uv-pos)/scale*2.;
  uv.x = .5-uv.x + STEPX*0.6;
	uv.y -= 1.;

	float seg = showNum(uv,number,true);
	return seg;
}

// Simplified cylinder intersection, used for drawing number with correct distorsion
float Cylinder_intersection(in vec3 rd, out vec3 iPoint)
{
  float a = dot(rd.xz,rd.xz);
  float disc = a;
  if (disc < 0.0) return -1.0;

  iPoint = rd * inversesqrt(disc);
  if (dot(iPoint,vec3(1.0,0.0,0.0)) < 0.0) return -1.0;

  if (iPoint.y > 0.3 ||
      iPoint.y < -0.3) return -1.0;
  return 1.0;
}

void main(void)
{
  vec2 uv = vTexCoord - vec2(0.5);
  pos.y = 1.0 - length(uv);
  if (pos.y < 0.5) {
      gl_FragColor = vec4(0.0,0.0,0.0,1.0);
      return;
  }
  pos.x = atan(uv.y,uv.x) / PI / 2.0;

  vec3 rgb = hsv2rgb_smooth(pos.x);
  vec3 vertPoint,horzPoint;

  float theta = pos.t * PI;
  float phi = (pos.s - 0.5)* 2.0 * PI;
  sin_theta = sin(theta);
  cos_theta = cos(theta);
  sin_phi = sin(phi);
  cos_phi = cos(phi);

  float leaders = clamp((2.0 - ruler()) * (abs(sin(PI*1.0*(ruler_pos.x - pos.x)))),0.0,2.0) ;
  float coarseGrid = max(clamp(1.0 - grid(vec2(18.0,18.0),0.1),0.0,1.0),
    clamp(1.0 - grid(vec2(2.0,2.0),0.04),0.0,1.0));
  float fineGrid = clamp(4.0 - grid(vec2(360.0,360.0),0.125),0.0,4.0);

  vec4 color = vec4(vec3(coarseGrid + fineGrid*0.10)*vec3(rgb) + vec3(leaders * 0.5 + leaders * fineGrid * 0.05) + pow(1.0 - sin(pos.y*PI) ,10.0),1.0);

  float phi_mouse = (pos.s - 0.25 - ruler_pos.x) * 2.0 * PI;
  vec3 rd = vec3(sin_theta * sin(phi_mouse), sin_theta * cos(phi_mouse), cos_theta);
  vec3 iPoint;
  if (Cylinder_intersection(rd,iPoint) > 0.0)
  {
    vec2 texCoord = vec2(acos(iPoint.y / 0.6) / PI,- iPoint.z +0.003);
    for (float i = 10.0; i < 180.0; i += 10.0 )
    {
      color += display(texCoord,vec2(0.425,cos((i/90.0)*PI*0.5)),vec2(0.05,0.075 * sin(i/90.0*PI*0.5)),90 - int(i));
    }
  }

  gl_FragColor = color;
}
