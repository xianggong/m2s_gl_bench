#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


void drawScene(void);
void initRendering();
void resizeWindow(int w, int h);

/* These variables set the dimensions of the rectanglar region we wish to view */
const double Xmin = 0.0, Xmax = 3.0;
const double Ymin = 0.0, Ymax = 3.0;

/*
 *  drawScene() handles the animation and the redrawing of the
 *  graphics window contents.
 */
void drawScene(void)
{
	/* Clear the rendering window */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
	static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
	static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};

	/* Draw a triangle */
	glBegin(GL_TRIANGLES);
	  	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		glVertex2f( 1.0, 1.0 );
	  	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
		glVertex2f( 2.0, 1.0 );
	  	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
		glVertex2f( 2.0, 2.0 );
	glEnd();

	/* Flush the pipeline */
	glFlush();

}

/* Initialize OpenGL's rendering modes */

void initRendering()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	/* Set light */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	static GLfloat pos0[4] = {2.0, 1.0, 0.1, 1.0};

	GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
	GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
	GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; //set the diffuse light to white

 	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
}

/* 
 * Called when the window is resized
 * w, h - width and height of the window in pixels.
 */
void resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	/* Define the portion of the window used for OpenGL rendering */
	glViewport( 0, 0, w, h );	/* View port uses whole window */

	/* 
	 * Set up the projection view matrix: orthographic projection
	 * Determine the min and max values for x and y that should appear in the window.
	 * The complication is that the aspect ratio of the window may not match the
	 * aspect ratio of the scene we want to view
	 */
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h )
	{
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else
	{
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}
	
	/*
	 * Now that we know the max & min values for x & y that should be visible in the window,
	 * we set up the orthographic projection 
	 */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1);
}


int main( int argc, char** argv )
{
	glutInit(&argc,argv);

	/* The image is not animated so single buffering is OK */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );

	/* Window position (from top corner), and size (width and hieght) */
	glutInitWindowPosition( 20, 60 );
	glutInitWindowSize( 360, 360 );
	glutCreateWindow( "Smooth Triangle Single Light" );

	/* Initialize OpenGL */
	initRendering();

	/* Set up the callback function for resizing windows */
	glutReshapeFunc( resizeWindow );

	/* Call this whenever window needs redrawing */
	glutDisplayFunc( drawScene );
	
	/* Start the main loop.  glutMainLoop never returns */
	glutMainLoop();

	return(0);
}
