#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


void drawScene(void);
void initRendering();
void resizeWindow(int w, int h);


/* These variables control the current mode */

int CurrentMode = 0;
const int NumModes = 5;

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

	/* Set drawing color to white */
	glColor3f( 1.0, 1.0, 1.0 );
	// glTranslatef(0.5, -0.5, 0.0);

	/* Draw a triangle */
	glBegin(GL_TRIANGLES);
		glColor3f( 1.0, 0.0, 0.0 );
		glVertex2f( 1.0, 1.0 );
		glColor3f( 0.0, 1.0, 0.0 );
		glVertex2f( 2.0, 1.0 );
		glColor3f( 0.0, 0.0, 1.0 );
		glVertex2f( 2.0, 2.0 );
	glEnd();

	/* Flush the pipeline */
	glFlush();

}

/* Initialize OpenGL's rendering modes */

void initRendering()
{
	glEnable ( GL_DEPTH_TEST );
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
	glutCreateWindow( "Smooth Triangle" );

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
