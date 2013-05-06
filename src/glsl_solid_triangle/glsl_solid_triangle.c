#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#define M_PI       3.14159265358979323846
#define true 1
#define false 0
  
// Data for triangle 1
float vertices1[] = {   -3.0f, -3.0f, -5.0f, 1.0f,
            3.0f, -3.0f, -5.0f, 1.0f,
            3.0f, 3.0f, -5.0f, 1.0f};
 
float colors1[] = { 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f};
 
 
// Shaders
char *vs = 
	"#version 150\n"

	"uniform mat4 viewMatrix, projMatrix;\n"

	"in vec4 position;\n"
	"in vec4 color;\n"

	"out vec4 Color;\n"
	"out vec4 Position;\n"

	"void main()\n"
	"{\n"
	"	Color = color;\n"
	"	gl_Position = projMatrix * viewMatrix * position ;\n"
	"	Position = gl_Position;\n"
	"}\n";

char *fs = 
	"#version 150\n"
	 
	"in vec4 Color;\n"
	"out vec4 outputF;\n"
	 
	"void main()\n"
	"{\n"
	"    outputF = Color;\n"
	"}\n";

// Program and Shader Identifiers
GLuint p,v,f;
 
// Vertex Attribute Locations
GLuint vertexLoc, colorLoc;
 
// Uniform variable Locations
GLuint projMatrixLoc, viewMatrixLoc;
 
// Vertex Array Objects Identifiers
GLuint vao[1];

//  Vertex/Color buffers
GLuint buffers[2];

// Transform feedback buffer
GLuint tfbuf[1];
 
// storage for Matrices
float projMatrix[16];
float viewMatrix[16];

// Dump data after vertex shader
int dump_vertex = 0;

void checkGlErrors( void )
{
	GLenum e = glGetError();
	while ( e != GL_NO_ERROR )
	{
		fprintf( stderr, "GL error: %s!\n", gluErrorString(e) );
		e = glGetError();
	}
}

 
/* For read/Write shaders */
char *textFileRead(char *fn)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");
		if (fp != NULL) 
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

				if (count > 0) {
					content = (char *)malloc(sizeof(char) * (count+1));
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char *fn, char *s)
{
	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

// ----------------------------------------------------
// VECTOR STUFF
//
 
// res = a cross b;
void crossProduct( float *a, float *b, float *res)
{ 
	res[0] = a[1] * b[2]  -  b[1] * a[2];
	res[1] = a[2] * b[0]  -  b[2] * a[0];
	res[2] = a[0] * b[1]  -  b[0] * a[1];
}
 
// Normalize a vec3
void normalize(float *a)
{
	float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);

	a[0] /= mag;
	a[1] /= mag;
	a[2] /= mag;
}
 
// ----------------------------------------------------
// MATRIX STUFF
//
 
// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix( float *mat, int size) 
{
	int i;
	// fill matrix with 0s
	for (i = 0; i < size * size; ++i)
	mat[i] = 0.0f;

	// fill diagonal with 1s
	for (i = 0; i < size; ++i)
	mat[i + i * size] = 1.0f;
}
 
//
// a = a * b;
//
void multMatrix(float *a, float *b)
{

	float res[16];
	int i;
	int j;
	int k;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j)
		{
			res[j*4 + i] = 0.0f;
			for (k = 0; k < 4; ++k)
				res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
		}
	}
	memcpy(a, res, 16 * sizeof(float));

}
 
// Defines a transformation matrix mat with a translation
void setTranslationMatrix(float *mat, float x, float y, float z)
{
	setIdentityMatrix(mat,4);
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
}
 
// ----------------------------------------------------
// Projection Matrix
//
 
void buildProjectionMatrix(float fov, float ratio, float nearP, float farP)
{
	float f = 1.0f / tan (fov * (M_PI / 360.0));

	setIdentityMatrix(projMatrix,4);

	projMatrix[0] = f / ratio;
	projMatrix[1 * 4 + 1] = f;
	projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
	projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
	projMatrix[2 * 4 + 3] = -1.0f;
	projMatrix[3 * 4 + 3] = 0.0f;
}
 
// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//
 
void setCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ)
{
	float dir[3], right[3], up[3];

	up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;

	dir[0] =  (lookAtX - posX);
	dir[1] =  (lookAtY - posY);
	dir[2] =  (lookAtZ - posZ);
	normalize(dir);

	crossProduct(dir,up,right);
	normalize(right);

	crossProduct(right,dir,up);
	normalize(up);

	float aux[16];

	viewMatrix[0]  = right[0];
	viewMatrix[4]  = right[1];
	viewMatrix[8]  = right[2];
	viewMatrix[12] = 0.0f;

	viewMatrix[1]  = up[0];
	viewMatrix[5]  = up[1];
	viewMatrix[9]  = up[2];
	viewMatrix[13] = 0.0f;

	viewMatrix[2]  = -dir[0];
	viewMatrix[6]  = -dir[1];
	viewMatrix[10] = -dir[2];
	viewMatrix[14] =  0.0f;

	viewMatrix[3]  = 0.0f;
	viewMatrix[7]  = 0.0f;
	viewMatrix[11] = 0.0f;
	viewMatrix[15] = 1.0f;

	setTranslationMatrix(aux, -posX, -posY, -posZ);

	multMatrix(viewMatrix, aux);
}
 
// ----------------------------------------------------
 
void changeSize(int w, int h)
{
 
	float ratio;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
	    h = 1;

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	ratio = (1.0f * w) / h;
	buildProjectionMatrix(53.13f, ratio, 1.0f, 30.0f);
}
 
void setupBuffers()
{
	glGenVertexArrays(1, vao);
	//
	// VAO for triangle
	//
	glBindVertexArray(vao[0]);
	
	// Generate two slots for the vertex and color buffers
	glGenBuffers(2, buffers);
	// bind buffer for vertices and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STREAM_DRAW);

	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// bind buffer for colors and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STREAM_DRAW);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glGenBuffers(1, tfbuf);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tfbuf[0]);
	glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(vertices1) + sizeof(colors1), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
 
void setUniforms()
{
	// must be called after glUseProgram
	glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);

	glUniformMatrix4fv(viewMatrixLoc,  1, false, viewMatrix);
}
 
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCamera(0,0,10,0,0,-5);

	glUseProgram(p);
	setUniforms();

	glBindVertexArray(vao[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tfbuf[0]);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_TRIANGLES);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, tfbuf[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tfbuf[0]);

	float *pos = (float *)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_WRITE);

	int i;
	if (pos != NULL)
	{
		for (i = 0; i < 3; ++i)
		{
			printf("Vertex #%d\n", i);
			printf("    Color: \t%f %f %f %f\n", pos[8 * i], pos[8 * i + 1], pos[8 * i + 2], pos[8 * i + 3]);
			printf("    Pos: \t%f %f %f %f\n", pos[8 * i + 4], pos[8 * i + 5], pos[8 * i + 6], pos[8 * i + 7]);
		}
	}

	if (pos != NULL && dump_vertex)
	{
		FILE * pFile;
		int n;

		pFile = fopen ("glsl_solid_triangle_vertex.txt","w");
		for (n=0 ; n<3 ; n++)
		{
			fprintf(pFile, "Vertex #%d\n", i);
			fprintf(pFile, "    Color: \t%f %f %f %f\n", pos[8 * i], pos[8 * i + 1], pos[8 * i + 2], pos[8 * i + 3]);
			fprintf(pFile, "    Pos: \t%f %f %f %f\n", pos[8 * i + 4], pos[8 * i + 5], pos[8 * i + 6], pos[8 * i + 7]);
		}
		fclose (pFile);
	}

	glutSwapBuffers();
}
 
void processNormalKeys(unsigned char key, int x, int y)
{

	if (key == 27)
	{
		glDeleteVertexArrays(1,vao);
		glDeleteProgram(p);
		glDeleteShader(v);
		glDeleteShader(f);
		exit(0);
	}
}
 
#define printOpenGLError() printOglError(__FILE__, __LINE__)
 
int printOglError(char *file, int line)
{
	//
	// Returns 1 if an OpenGL error occurred, 0 otherwise.
	//
	GLenum glErr;
	int retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}
 
void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}
 
void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}
 
GLuint setupShaders()
{
	GLuint p,v,f;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	glCompileShader(v);
	glCompileShader(f);

	printShaderInfoLog(v);
	printShaderInfoLog(f);

	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);
	
	const char* varyings[2] = { "Color", "Position" };
	glTransformFeedbackVaryings(p, 2, varyings, GL_INTERLEAVED_ATTRIBS);

	glBindFragDataLocation(p, 0, "outputF");
	glLinkProgram(p);
	printProgramInfoLog(p);

	vertexLoc = glGetAttribLocation(p,"position");
	colorLoc = glGetAttribLocation(p, "color"); 

	projMatrixLoc = glGetUniformLocation(p, "projMatrix");
	viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");

	return(p);
}
 
int main(int argc, char **argv)
{
	int argi;
	for (argi = 0; argi < argc; ++argi)
	{
		/* Dump intermediate ELF files */
		if (!strcmp(argv[argi], "-v"))
		{
			argi++;
			dump_vertex = 1;
			continue;
		}		
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("GLSL Solid Triangle");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);

	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
	    printf("Ready for OpenGL 3.3\n");
	else {
	    printf("OpenGL 3.0 not supported\n");
	    exit(1);
	}

	p = setupShaders();

	setupBuffers();

	glutMainLoop();

	return 0;
}