include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am

include $(top_srcdir)/ada.am

noinst_PROGRAMS = ada_sp01
ada_sp01_SOURCES = sp01.adb config.h sptest.adb sptest.ads
ada_sp01_SOURCES += init.c

ada_sp01$(EXEEXT): sp01.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

scndir = $(rtems_ada_testsdir)
dist_scn_DATA = ada_sp01.scn

