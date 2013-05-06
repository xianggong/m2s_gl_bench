if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/gl-solid-triangle-single-light

__top_builddir__bin_gl_solid_triangle_single_light_SOURCES = \
	gl_solid_triangle_single_light.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
