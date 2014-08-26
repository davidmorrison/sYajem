#include <fstream>
#include <iostream>
#include <cmath>
#include "boost/foreach.hpp"
#include "boost/range/adaptors.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include "boost/program_options.hpp" 
#include "HepMC/GenEvent.h"
#include "TFile.h"
#include "TH1F.h"
#include "fastjet/ClusterSequence.hh"
#include "HepMC/PythiaWrapper.h"
#include "HepMC/IO_HEPEVT.h"

using namespace std;
using namespace HepMC;
using namespace fastjet;
namespace po = boost::program_options;

struct IsStateFinal 
{
  /// returns true if the GenParticle does not decay
  bool operator()(const HepMC::GenParticle* p) 
  { 
    return (!p->end_vertex() && p->status() == 1);
  }
};

int 
main(int argc, char** argv)  
{ 
  int ranseed;
  double sqrts;
  int nevents;
  bool csv;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("ranseed,r", po::value<int>(&ranseed)->default_value(55122), 
     "random number seed")
    ("sqrts,s", po::value<double>(&sqrts)->default_value(200.0), 
     "center of mass energy")
    ("nevents,n", po::value<int>(&nevents)->default_value(100000), 
     "number of events to generate")
    ("csv,c", po::value<bool>(&csv)->default_value(false), 
     "generate CSV output");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);   

  IO_HEPEVT hepevtio; 
  HepMC::HEPEVT_Wrapper::set_max_number_entries(4000);
  HepMC::HEPEVT_Wrapper::set_sizeof_real(8);

  pysubs.msel = 1;
  // set random number seed (mandatory!)
  pydatr.mrpy[0]=ranseed;

  // Silence the Pythia welcome banner
  pydat1.mstu[11]=12345;
  pydat1.mstu[12]=0;

  // Set CKIN(3) to some a reasonable minimum value
  pysubs.ckin[2] = 5.0;

  // Call pythia initialization
  call_pyinit("CMS", "p", "p", sqrts);
  
  TFile *root_file = new TFile("pythia.root", "RECREATE");
  boost::ptr_map<float, TH1F> root_hists;
  for (int i = 2; i < 8; ++i)
    {
      string name = "inclusive_jets_0" + to_string(i);
      float key = i/10.0;
      TH1F *hist = new TH1F(name.c_str(), name.c_str(), 200, 0.0, sqrts/2.0);
      root_hists[key] = *hist;
    }
  TH1 *pions = new TH1F("pions", "pions", 200, 0.0, sqrts/2.0);
  TH1 *n_events = new TH1F("N", "number of events", 1, -0.5, 0.5);

  ofstream out_csv;
  if (csv)
    {
      out_csv.open("partons.csv");
      out_csv << "id1,id2,x1,x2,roots,Q,y,beta,cos_theta" << endl;
    }

  ofstream out_dat("partons.dat");

  for ( int i = 1; i <= nevents; i++ ) {
    if (i% 1000 == 0)
      {
	cout << "working on event: " << i << endl;
      }
    call_pyevnt();      // generate one event with Pythia

    double y = pyint1.vint[21];
    double beta = tanh(y);

    if (csv)
      {
	out_csv << pyint1.mint[14] << ","
	      << pyint1.mint[15] << ","
		<< pyint1.vint[40] << ","
		<< pyint1.vint[41] << ","
		<< pyint1.vint[42] << ","
		<< pyint1.vint[50] << ","
		<< y << ","
		<< beta << ","
		<< pyint1.vint[22] 
		<< endl;
      }
  
    out_dat << pyint1.mint[14] << " "
	    << pyint1.mint[15] << " "
	    << pyint1.vint[40] << " "
	    << pyint1.vint[41] << " "
	    << pyint1.vint[42] << " "
	    << pyint1.vint[50] << " "
	    << y << " "
	    << beta << " "
	    << pyint1.vint[22] 
	    << endl;

    // Populate the HEPEVT common from the PYJETS common
    call_pyhepc(1);
    
    // Create a HepMC event from the PYJETS common.
    GenEvent *eptr = new GenEvent(*hepevtio.read_next_event());

    // Grab all the final state particles and run fastjet on them
    list<GenParticle*> finalstateparticles;
    HepMC::copy_if(eptr->particles_begin(), eptr->particles_end(), 
		   back_inserter(finalstateparticles), IsStateFinal());
    vector<PseudoJet> particles;
    BOOST_FOREACH(GenParticle *pptr, finalstateparticles)
      {
	int pid = pptr->pdg_id();
	const FourVector &momentum = pptr->momentum();
	double px = momentum.px();
	double py = momentum.py();
	double pz = momentum.pz();
	double E =  momentum.e();
	particles.push_back(PseudoJet(px, py, pz, E));

	if (abs(pid) == 211)
	  {
	    pions->Fill(momentum.perp());
	  }
      }
    BOOST_FOREACH(float R, root_hists | boost::adaptors::map_keys)
      {
	JetDefinition jet_def(antikt_algorithm, R);
	ClusterSequence cs(particles, jet_def);
	vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());	  
	BOOST_FOREACH(PseudoJet jet, jets)
	  {
	    root_hists[R].Fill(jet.pt());
	  }
      }

    delete eptr;
  }

  root_file->Write();
}

