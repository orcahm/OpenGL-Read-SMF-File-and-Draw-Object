#version 150

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

void main()
{
   vec3 angles = radians( vec3(10.0, 0.0, 0.0) );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    mat4 rx = mat4( 1.0,  0.0,  0.0, 0.0,
					0.0,  c.x,  s.x, 0.0,
					 0.0, -s.x,  c.x, 0.0,
					 0.0,  0.0,  0.0, 1.0 );

	mat4 trans= mat4( 1.0,  0.0,  0.0, 0.0,
						0.0,  1.0,  0.0, 0.0,
						0.0, 0.0,  1.0, 0.0,
						0.0,  -0.3,  -0.2, 1.0 );


	
    gl_Position = trans*rx*vPosition;
	color = vColor;
}
