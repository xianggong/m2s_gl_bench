if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/glsl-solid-triangle

__top_builddir__bin_glsl_solid_triangle_SOURCES = \
	glsl_solid_triangle.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
