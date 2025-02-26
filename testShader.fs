#version 330 core

float functionOne(float x)
{
    float x2 = x*x;
    float x4 = x2*x2;
    float x6 = x4*x2;
    float fa = (4.0/9.0);
    float fb = 17.0/9.0;
    float fc = 22.0/9.0;
    float y = fa*x6-fb*x4+fc*x2;
    return y;
}

#define t iTime
#define r iResolution.xy
void main() {
    

    vec3 c;
	float l,z=t;
	for(int i=0;i<3;i++) {
		vec2 uv,p=gl_FragCoord.xy/r;
		uv=p;
		p-=.5;
		p.x*=r.x/r.y;
		z+=.07;
		l=length(p);
		uv+=p/l*(sin(z)+1.)*abs(sin(l*9.-z-z));
		c[i]=.01/length(mod(uv,1.)-.5);
	}
	gl_FragColor=vec4(c/l,t);
}