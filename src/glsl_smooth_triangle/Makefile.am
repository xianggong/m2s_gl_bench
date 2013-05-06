if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/glsl-smooth-triangle

__top_builddir__bin_glsl_smooth_triangle_SOURCES = \
	glsl_smooth_triangle.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
