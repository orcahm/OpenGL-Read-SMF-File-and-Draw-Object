#version 150 

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform vec3 theta;
uniform mat4 traslation;
uniform mat4 scale;

void main() 
{
	  vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );
	
	 mat4 ry = mat4( c.y, 0.0, -s.y, 0.0,
					0.0, 1.0,  0.0, 0.0,
					s.y, 0.0,  c.y, 0.0,
					0.0, 0.0,  0.0, 1.0 );

    // Workaround for bug in ATI driver
    ry[1][0] = 0.0;
    ry[1][1] = 1.0;

	
    gl_Position = traslation*ry*scale*vPosition;
    color = vColor;
} 
