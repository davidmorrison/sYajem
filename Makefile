HEPMC = $(HOME)/local
BOOST = /opt/local
FC = gfortran 
FFLAGS = -g -O0 -fno-range-check -ffixed-line-length-180
CXX = g++
CXXFLAGS = -g -O2 -I$(BOOST)/include -I$(HEPMC)/include `root-config --cflags` `fastjet-config --cxxflags`

PYTHIA_OBJ = pythia-6.4.28.o
PYTHIA_YAJEM_OBJ = pythia_yajem.o

default: pythia_pp sYajem

clean: 
	rm -f pythia_pp sYajem *.o *~

sYajem: sYajem.o trajectory_manager.o interaction_manager.o yajem_manager.o gyadat.o gyadat1.o gpydatr.o gpydat1.o $(PYTHIA_YAJEM_OBJ) yajem_hepmc.o
	$(CXX) -g -o $@ $^ -L$(HEPMC)/lib -lHepMC -L/opt/local/lib -lboost_filesystem-mt -lboost_program_options-mt -lboost_regex-mt -lboost_system-mt `root-config --libs` `fastjet-config --libs --plugins` -L/usr/local/gfortran/lib -lgfortran -lHepMC -lHepMCfio
	install_name_tool -change libHepMC.4.dylib $(HEPMC)/local/lib/libHepMC.4.dylib $@
	install_name_tool -change libHepMCfio.4.dylib $(HEPMC)/local/lib/libHepMCfio.4.dylib $@

pythia_pp: pythia_pp.o $(PYTHIA_OBJ)
	$(CXX) -g -o $@ $^ -L$(HOME)/local/lib `root-config --libs` `fastjet-config --libs` -lHepMC -lHepMCfio -L/usr/local/gfortran/lib -lgfortran -L/opt/local/lib -lboost_program_options-mt
	install_name_tool -change libHepMC.4.dylib $(HOME)/local/lib/libHepMC.4.dylib $@
	install_name_tool -change libHepMCfio.4.dylib $(HOME)/local/lib/libHepMCfio.4.dylib $@
