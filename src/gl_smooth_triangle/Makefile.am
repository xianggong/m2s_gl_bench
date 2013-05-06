if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/gl-smooth-triangle

__top_builddir__bin_gl_smooth_triangle_SOURCES = \
	gl_smooth_triangle.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
