if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/gl-smooth-triangle-single-light

__top_builddir__bin_gl_smooth_triangle_single_light_SOURCES = \
	gl_smooth_triangle_single_light.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
