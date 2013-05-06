if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/glsl-solid-triangles-overlap

__top_builddir__bin_glsl_solid_triangles_overlap_SOURCES = \
	glsl_solid_triangles_overlap.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
