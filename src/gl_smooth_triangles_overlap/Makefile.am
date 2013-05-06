if HAVE_OPENGL

bin_PROGRAMS = $(top_builddir)/bin/gl-smooth-triangles-overlap

__top_builddir__bin_gl_smooth_triangles_overlap_SOURCES = \
	gl_smooth_triangles_overlap.c

AM_CPPFLAGS = @TEST_OPENGL_INCLUDES@

LDADD = \
	-lGLEW -lGLU -lGL -lglut -lm

endif
