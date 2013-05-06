AC_INIT([test-opengl], [0.1])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

#############
# Test OpenGL
#############
have_opengl=yes

AC_CHECK_HEADER([GL/glut.h], [], [have_opengl=no])
AC_CHECK_HEADER([GL/gl.h], [], [have_opengl=no])
AC_CHECK_HEADER([GL/glu.h], [], [have_opengl=no])
AC_CHECK_HEADER([GL/glew.h], [], [have_opengl=no])
AC_CHECK_LIB([glut], [glutInit], [], [have_opengl=no])
AC_CHECK_LIB([GL], [glBegin], [], [have_opengl=no])
AC_CHECK_LIB([GLU], [gluOrtho2D], [], [have_opengl=no])
AC_CHECK_LIB([GLEW], [glewInit], [], [have_opengl=no])

if test "x$have_opengl" = "xno"
	then
		AC_MSG_WARN([

	The OpenGL/glut/glew libraries are not installed in your system. These
	libraries are needed to compile this test suite

	In Ubuntu, you can install the package by using the following command:

		sudo apt-get install freeglut3-dev libglew-dev]
	)
fi

# Define macro for Automake and gcc
AM_CONDITIONAL(HAVE_OPENGL, test x$have_opengl == xyes )
if test x$have_opengl = xyes 
then
	AC_DEFINE(HAVE_OPENGL, [1])
fi

##########
# Includes
##########

TEST_OPENGL_INCLUDES=" \
\
-I\$(top_srcdir)/includes \
\
"

AC_SUBST(TEST_OPENGL_INCLUDES)

###########
# Makefiles
###########

AC_CONFIG_FILES([
Makefile
src/Makefile
src/gl_solid_triangle/Makefile
src/gl_solid_triangle_single_light/Makefile
src/gl_solid_triangles_overlap/Makefile
src/gl_solid_triangles_overlap_lights/Makefile
src/gl_smooth_triangle/Makefile
src/gl_smooth_triangle_single_light/Makefile
src/gl_smooth_triangles_overlap/Makefile
src/gl_smooth_triangles_overlap_lights/Makefile
src/glsl_solid_triangle/Makefile
src/glsl_solid_triangles_overlap/Makefile
src/glsl_smooth_triangle/Makefile
src/glsl_smooth_triangles_overlap/Makefile
])
AC_OUTPUT
