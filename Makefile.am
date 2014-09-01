AUTOMAKE_OPTIONS = foreign

F77=$(FC)
FFLAGS=$(FCFLAGS)

AM_FFLAGS = -fno-range-check -ffixed-line-length-180

AM_CPPFLAGS = \
  -I$(includedir) \
  $(BOOST_CPPFLAGS) \
  $(HEPMC_CPPFLAGS) \
  `root-config --cflags` \
  `fastjet-config --cxxflags`

noinst_HEADERS = \
  PyDat1.h \
  PyDatr.h \
  YaDat.h \
  YaDat1.h \
  interaction_manager.h \
  trajectory_manager.h \
  yajem_manager.h

bin_PROGRAMS = sYajem

sYajem_SOURCES = \
  interaction_manager.cc \
  trajectory_manager.cc \
  sYajem.cc \
  yajem_hepmc.cc \
  yajem_manager.cc \
  gpydat1.f \
  gpydatr.f \
  gyadat.f \
  gyadat1.f \
  pythia_yajem.f

sYajem_LDFLAGS = `root-config --libs` `fastjet-config --libs` $(HEPMC_LDFLAGS)

sYajem_LDADD = -lHepMC -lHepMCfio $(FLIBS)

clean-local:
	rm -f $(BUILT_SOURCES)

