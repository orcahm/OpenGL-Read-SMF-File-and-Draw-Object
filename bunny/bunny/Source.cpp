#define _CRT_SECURE_NO_DEPRECATE

#include "Angel.h"
#include "glui.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;


FILE * fp;

int verticesCount = 0;
int facesCount = 0;
point4 *vertices;
GLuint *face;

GLuint vao[3];

GLUI *glui;
int   main_window;

color4 colors[15000];
point4 points[15000];
color4 colors1[7392];
point4 points1[7392];

// RGBA olors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(0.0, 1.0, 1.0, 1.0)   // cyan
};

// Viewing transformation parameters

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat  dr = 5.0 * DegreesToRadians;

GLuint  model_view;  // model-view matrix uniform shader variable location

					 // Projection transformation parameters

GLfloat  left = -1.0, right = 1.0;
GLfloat  bottom = -1.0, top = 1.0;
GLfloat  zNear = 0.5, zFar = 3.0;

GLuint  projection; // projection matrix uniform shader variable location

mat4 traslationbunny = mat4(vec4(1.0, 0.0, 0.0, 0.0),
							vec4(0.0, 1.0, 0.0, 0.0),
							vec4(0.0, 0.0, 1.0, 0.0),
							vec4(-0.5, 0.0, 0.0, 1.0));

mat4 traslationteapot = mat4(vec4(1.0, 0.0, 0.0, 0.0),
							vec4(0.0, 1.0, 0.0, 0.0),
							vec4(0.0, 0.0, 1.0, 0.0),
							vec4(0.5, -0.25, 0.0, 1.0));

mat4 scalebunny = mat4(vec4(0.4, 0.0, 0.0, 0.0),
						vec4(0.0, 0.4, 0.0, 0.0),
						vec4(0.0, 0.0, 0.4, 0.0),
						vec4(0.0, 0.0, 0.0, 1.0));

mat4 scaleteapot = mat4(vec4(0.4, 0.0, 0.0, 0.0),
						vec4(0.0, 0.4, 0.0, 0.0),
						vec4(0.0, 0.0, 0.4, 0.0),
						vec4(0.0, 0.0, 0.0, 1.0));


GLuint myTransteapot;
GLuint myTransbunny;
GLuint myscaleteapot;
GLuint myscalebunny;
GLuint program;
GLuint program1;
GLuint program2;


point4 Cir[360];
color4 CirColur[360];

GLfloat  Thetabunny[3] = { 0.0, 0.0, 0.0 };
GLuint  thetabunny;
float rotSpeedBunny = 0.02;
GLfloat  Thetateapot[3] = { 0.0, 0.0, 0.0 };
GLuint  thetateapot;
float rotSpeedTeapot = 0.02;
//----------------------------------------------------------------------------

int Index = 0;

void
counter(char *smf) {
	char current;
	int  count = 0, count1 = 0;
	verticesCount = 0;
	facesCount = 0;

	fp = fopen(smf, "r");

	if (!fp)
		printf("dosya açýlamadý!");

	while (!feof(fp)) {
		current = getc(fp);
		if (current == 'v') {
			verticesCount++;
		}
		else if (current == 'f') {
			facesCount++;
		}

	}

	fclose(fp);

}

void
read(char *smf) {
	char current;
	int  count = 0, count1 = 0;

	vertices = (point4 *)malloc(sizeof(point4) * verticesCount);
	face = (GLuint *)malloc(sizeof(GLuint) * 3 * facesCount);
	fp = fopen(smf, "r");

	if (!fp)
		printf("dosya okunamadý");

	while (!feof(fp)) {
		current = getc(fp);
		if (current == 'v') {
			fscanf(fp, "%f %f %f", &vertices[count][0], &vertices[count][1], &vertices[count][2]);
			vertices[count][3] = 1.0;
			count += 1;
		}
		else if (current == 'f') {

			fscanf(fp, "%d %d %d", &face[count1], &face[count1 + 1], &face[count1 + 2]);
			face[count1] -= 1;
			face[count1 + 1] -= 1;
			face[count1 + 2] -= 1;
			count1 += 3;
		}
	}

	fclose(fp);

}

//----------------------------------------------------------------------------

void
bunny(GLuint a, GLuint b, GLuint c,int color)
{
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
}

//----------------------------------------------------------------------------

void
creatbunny()
{
	int i;
	for (i = 0; i < facesCount;i++) {
		bunny(face[Index],face[Index+1],face[Index+2],3);
	}
	//
}


//----------------------------------------------------------------------------

void
Teapot(GLuint a, GLuint b, GLuint c, int color)
{
	colors1[Index] = vertex_colors[color]; points1[Index] = vertices[a]; Index++;
	colors1[Index] = vertex_colors[color]; points1[Index] = vertices[b]; Index++;
	colors1[Index] = vertex_colors[color]; points1[Index] = vertices[c]; Index++;
}

void
creatTeapot() {

	int i;
	Index = 0;
	for (i = 0; i < facesCount; i++) {
		Teapot(face[Index], face[Index + 1], face[Index + 2], 5);
	}
	
}

void
creatcircle(){
	
	GLfloat  x, z;
	for (int i = 0; i<360; i++) {
		x = 0.8f*cos(i*2.0f*3.14f / 360.0f);
		z = 0.8f*sin(i*2.0f*3.14f / 360.0f);

		Cir[i][0] = x; Cir[i][1] = 0.0 ; Cir[i][2] = z; Cir[i][3] = 1.0;
		CirColur[i] = vertex_colors[1];
	}

}

// OpenGL initialization
void
init()
{
	counter("bunny.smf");
	read("bunny.smf");
	creatbunny();

	// Create and initialize a buffer object
	

	// Create a vertex array object
	glGenVertexArrays(3, vao);

	glBindVertexArray(vao[0]);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	program = InitShader("vshader41.glsl", "fshader41.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	myscalebunny = glGetUniformLocation(program, "scale");
	myTransbunny = glGetUniformLocation(program, "traslation");
	thetabunny = glGetUniformLocation(program, "theta");

	counter("teapot.smf");
	read("teapot.smf");
	creatTeapot();

	glBindVertexArray(vao[1]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points1) + sizeof(colors1),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points1), points1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points1), sizeof(colors1), colors1);

	// Load shaders and use the resulting shader program
	program1 = InitShader("vshader41.glsl", "fshader41.glsl");
	glUseProgram(program1);

	// set up vertex arrays
	GLuint vPosition1 = glGetAttribLocation(program1, "vPosition");
	glEnableVertexAttribArray(vPosition1);
	glVertexAttribPointer(vPosition1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor1 = glGetAttribLocation(program1, "vColor");
	glEnableVertexAttribArray(vColor1);
	glVertexAttribPointer(vColor1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points1)));

	thetateapot = glGetUniformLocation(program1, "theta");
	myTransteapot = glGetUniformLocation(program1, "traslation");
	myscaleteapot = glGetUniformLocation(program1, "scale");

	creatcircle();

	glBindVertexArray(vao[2]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cir) + sizeof(CirColur),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Cir), Cir);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Cir), sizeof(CirColur), CirColur);

	// Load shaders and use the resulting shader program
	program2 = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program2);

	// set up vertex arrays
	GLuint vPosition2 = glGetAttribLocation(program2, "vPosition");
	glEnableVertexAttribArray(vPosition2);
	glVertexAttribPointer(vPosition2, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor2 = glGetAttribLocation(program2, "vColor");
	glEnableVertexAttribArray(vColor2);
	glVertexAttribPointer(vColor2, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(Cir)));
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glUniformMatrix4fv(myTransbunny, 1, GLU_FALSE, traslationbunny);
	glUniformMatrix4fv(myscalebunny, 1, GLU_FALSE, scalebunny);
	glUniform3fv(thetabunny, 1, Thetabunny);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 15000);
	
	/*glUseProgram(program2);
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 360 * 3);*/

	glUseProgram(program1);
	glUniformMatrix4fv(myTransteapot, 1, GLU_FALSE, traslationteapot);
	glUniformMatrix4fv(myscaleteapot, 1, GLU_FALSE, scaleteapot);
	glUniform3fv(thetateapot, 1, Thetateapot);
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, 7392);




	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	
	case 't':
	case 'T':
		rotSpeedTeapot *= -1;
		break;

	case 'b':
	case 'B':
		rotSpeedBunny *= -1;
		break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

//----------------------------------------------------------------------------

void
idle() {
	
	Thetabunny[1] += rotSpeedBunny;

	if (Thetabunny[1] > 360.0) {
		Thetabunny[1] -= 360.0;
	}

	Thetateapot[1] += rotSpeedTeapot;

	if (Thetateapot[1] > 360.0) {
		Thetateapot[1] -= 360.0;
	}

	glutPostRedisplay();
}


int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(614.4, 512);
	//glutInitContextVersion(3, 2);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	main_window = glutCreateWindow("Color Cube");
	glewExperimental = true;
	
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}