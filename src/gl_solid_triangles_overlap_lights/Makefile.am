if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/gl-solid-triangles-overlap-lights

__top_builddir__bin_gl_solid_triangles_overlap_lights_SOURCES = \
	gl_solid_triangles_overlap_lights.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
