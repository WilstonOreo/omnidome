
#ifdef GL_ES
precision mediump float;
#endif
 
uniform float time;
uniform vec2 resolution;

#define N 30
void main( void ) 
{
  vec4 clr = vec4(0.0,0.0,0.0,0.0);
  vec2 v = (gl_FragCoord.xy / resolution - vec2(0.5)) * 5.0;

	float rsum = 0.0;
	float pi2 = 3.14 * 2.0;
	float C = cos(1.0/600.0* pi2+time*0.01);
	float S = sin(1.0/444.0* pi2+time*(2.0+cos(time*0.005))*0.02);
	vec2 shift = vec2( 0.40, 1.00);
	float zoom = (11.1/28.*1.0 +  1.0);
	
	for ( int i = 0; i < N; i++ ){
		float rr = dot(v,v);
		if ( rr > 1.0 ){
			rr = 1.0/rr;
			v *= rr;
		}
		rsum *= 0.995;
		rsum += rr * 4.0;
		
		v = vec2( C*v.x-S*v.y, S*v.x+C*v.y ) * zoom + shift;
	}
	
	float col = rsum ;

	gl_FragColor = vec4( 
  cos(0.417*col), 
  cos(0.163*col), 
  cos(0.5*col), 1.0 );

}
