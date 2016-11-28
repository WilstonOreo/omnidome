// mplanck
// Tested on 13-inch Powerbook
// Tested on Late 2013 iMac
// Tested on Nvidia GTX 780 Windows 7

// **************************************************************************
// CONSTANTS

uniform float cam_roll;
uniform float cam_pitch;
uniform float cam_yaw;


////////// shaders/direction.h //////////
////////// shaders/util.h //////////

const float PI = 3.14159265358979323846264;

struct Ray
{
  vec3 org;
  vec3 dir;
};

/// Convert degrees to radians
float deg2rad(in float deg)
{
  return deg * PI / 180.0;
}


/// Convert degrees to radians
float rad2deg(in float rad)
{
  return rad / PI * 180.0;
}

float sqr(in float a)
{
  return a*a;
}

/// Calculates the rotation matrix of a rotation around X axis with an angle in radians
mat3 rotateAroundX( in float angle )
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3(1.0,0.0,0.0,
              0.0,  c, -s,
              0.0,  s,  c);
}

/// Calculates the rotation matrix of a rotation around Y axis with an angle in radians
mat3 rotateAroundY( in float angle )
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3(  c,0.0,  s,
              0.0,1.0,0.0,
               -s,0.0,  c);
}

/// Calculates the rotation matrix of a rotation around Z axis with an angle in radians
mat3 rotateAroundZ( in float angle )
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3(  c, -s,0.0,
                s,  c,0.0,
              0.0,0.0,1.0);
}

/// Calculate rotation by given yaw and pitch angles (in degrees!)
mat3 rotationMatrix(in float yaw, in float pitch, in float roll)
{
  return rotateAroundZ(deg2rad(yaw)) *
         rotateAroundY(deg2rad(-pitch)) *
         rotateAroundX(deg2rad(roll));
}

float solveQuadraticEquation(
    in float a,
    in float b,
    in float c)
{
  float disc = b*b - 4.0 * a * c;
  if (disc < 0.0) return -1.0;

  float distSqrt = sqrt(disc);
  float q;
  if (b < 0.0)
  {
    q = (-b - distSqrt)*0.5;
  }
  else
  {
    q = (-b + distSqrt)*0.5;
  }
  if (q == 0.0) return -1.0;

  float t = c / q;
  if (t < 0.0)
    t = q / a;

  return t;
}

// Get fisheye camera ray from screen coordinates
float fisheye_direction(in vec2 screenCoord, in vec2 res, out vec3 rd)
{
	vec2 uv = screenCoord.xy / res;
	uv = uv - 0.5;
  float phi = atan(uv.x,uv.y);
  float l = length(uv);

  if (l > 0.5)
  {
    return -1.0;
  }
  float theta  = l * PI;
  rd = normalize(vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta)));
  return 1.0;
}

float direction(in vec2 screenCoord, in vec2 res, out vec3 rd)
{
  return fisheye_direction(screenCoord,res,rd);
}




// Get fisheye camera ray from screen coordinates with rotation
float direction(in vec2 screenCoord, in vec2 res, float rotX, float rotY, float rotZ, out vec3 rd)
{
  if (direction(screenCoord,res,rd) < 0.0)
  {
    return -1.0;
  }
  rd *= rotateAroundZ(rotZ)*rotateAroundY(rotY)*rotateAroundX(rotX);
  return 1.0;
}



uniform vec2 iMouse;
uniform vec2 iResolution;
uniform float iGlobalTime;

float box(vec3 p) {
	return length(max(abs(p)-vec3(.5),0.0)) - 0.15;
}

vec3 rot(vec3 p, float f) {
	float s = sin(f);
	float c = cos(f);
	p.xy *= mat2(c, -s, s, c);
	p.yz *= mat2(c, -s, s, c);
	return p;
}


vec3 trans(vec3 p, out float rotout) {
	p.zx += iGlobalTime*8.0;

	vec3 b = vec3(4.);
	vec3 rep = floor(p/b);

	p = mod(p,b)-0.5*b;

	rotout = iGlobalTime*1.88 + (rep.x+rep.z+rep.y)*0.3;
	p = rot(p, rotout);
	return p;
}

float scene(vec3 p) {
	float dummy;
	return box(trans(p,dummy));
}


vec3 normal(vec3 p, float d) {
	vec3 e = vec3(0.04,.0,.0);
	return normalize(vec3(
		scene(p+e.xyy)-d,
		scene(p+e.yxy)-d,
		scene(p+e.yyx)-d));

}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 xy = fragCoord.xy / iResolution.xy - vec2(0.5,0.5);
	xy.y *= -iResolution.y / iResolution.x;

	float time = iGlobalTime*0.5;
	vec3 ro = 1.5*normalize(vec3(cos(time),cos(time)*1.2,sin(time)));
    vec3 eyed = normalize(vec3(0.0) - ro);
    vec3 ud = normalize(cross(vec3(0.0,1.0,0.0), eyed));
    vec3 vd = normalize(cross(eyed,ud));

	float fov = 3.14 * 0.7;


	float f = fov * length(xy);
	vec3 rd;
	direction(fragCoord,iResolution,deg2rad(cam_yaw),deg2rad(cam_pitch),deg2rad(cam_roll),rd);
	vec3 p = ro + rd;

	float dall,d;
	for(int i = 0; i < 16; i++) {
		d = scene(p);
		if(d < 0.06) break;
		p += d*rd;
		dall += d;
	}

	vec3 bg = normalize(p).zzz + 0.1;

	if(d < 0.06) {
		vec3 n = normal(p,d);
		vec3 col = vec3(dot(vec3(0.0,0.0,1.0), n));
		float objrot;
		vec3 objp = trans(p,objrot);
		vec3 objn = abs(rot(n,objrot));

		vec2 uv =
			(objn.y > 0.707) ? vec2(objp.zx) :
			(objn.x > 0.707) ? vec2(objp.zy) :
							   vec2(objp.xy) ;
				vec3 hl = smoothstep(0.6, 1.0, col);
		col *= clamp(0.6, 0.0, 1.0);

		col = col + hl*.4;
		float fog = clamp(dall/mix(90.0,40.0,((rd.z+1.0)*0.5)), 0.0, 1.0);

		fragColor = vec4(mix(col, bg, fog),1.0);
	}
	else {
		fragColor = vec4(bg, 1.0);
	}

}
//--------------------------------------------------------------------------

void main()
{
   mainImage(gl_FragColor,gl_FragCoord.xy);
}
