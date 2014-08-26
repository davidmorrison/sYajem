#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "HepMC/PythiaWrapper.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"

// These scoped_ptr's sit in file scope, so their destructors get
// called at program termination.
boost::scoped_ptr<HepMC::IO_GenEvent> ascii_io; 
boost::scoped_ptr<HepMC::IO_HEPEVT> hepevtio;

using namespace std;

extern "C" 
void yajem_hepmc_ (void)
{
  // On first call, initialize objects which talk to the output file
  // and to the HEPEVT common block.
  if (ascii_io == 0) 
    {
      ascii_io.reset(new HepMC::IO_GenEvent("yajem.hepmc",std::ios::out));
      hepevtio.reset(new HepMC::IO_HEPEVT());
    }

  // Populate the HEPEVT common from the PYJETS common
  call_pyhepc(1);

  // Fetch info from the HEPEVT common and write to the output file.
  HepMC::GenEvent *evt = hepevtio->read_next_event();

//  // use GenEvent::vertex_iterator to fill a list of all 
//  // vertices in the event
//  cout << "New event:" << endl;
//  evt->print(cout);
//  for ( HepMC::GenEvent::vertex_iterator v = evt->vertices_begin();
//	v != evt->vertices_end(); ++v ) 
//    {
//      if ((*v)->id() == -1) 
//	{
//	  cout << " This vertex has " << (*v)->particles_in_size() << " incoming particles" << endl;
//	}
//    }

  *ascii_io << evt;
  delete evt; 
}

