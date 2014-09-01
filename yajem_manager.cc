#include <cmath>
#include <list>
#include <string>
#include "yajem_manager.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1F.h"
#include "trajectory_manager.h"
#include "boost/foreach.hpp"
#include "boost/range/adaptors.hpp"
#include "HepMC/PythiaWrapper.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"
#include "fastjet/ClusterSequence.hh"

#include "PyDat1.h"
#include "PyDatr.h"
#include "YaDat1.h"
#include "YaDat.h"

using namespace std;
using namespace HepMC;
using namespace fastjet;

PyDat1 m_pydat1;
PyDatr m_pydatr;
YaDat1 m_yadat1;
YaDat m_yadat;

extern "C"
{
  void py2ent_(int&, int&, int&, double&);
  void pyshow_(int&, int&, double&);
  void pyjoin_(int&, int&);
  void pyexec_(void);
  void pyedit_(int&);
  void pyrobo_(int&,int&,double&,double&,double&,double&,double&);
  void yajem_hepmc_(void);
}

struct IsStateFinal 
{
  // returns true if the GenParticle does not decay
  bool operator()( const HepMC::GenParticle* p ) 
  { 
    return (!p->end_vertex() && p->status() == 1);
  }
};

bool
check_event()
{
  call_pyhepc(1);

  bool found_CMShower = false;
  for (int i = 0; i < HepMC::HEPEVT_Wrapper::number_entries(); ++i)
    {
      int idx = i+1;
      if (HepMC::HEPEVT_Wrapper::id(idx) == 94)
	{
	  found_CMShower = true;
	  break;
	}
    }
  return found_CMShower;
}


yajem_manager::yajem_manager(const std::string& _root, int _ranseed)
  : root(_root), ranseed(_ranseed)
{
  event_number = -1;

  string A, B, C;
  A = root + "_A.hepmc";
  B = root + "_B.hepmc";
  C = root + ".root";
  file[0].open(A.c_str());
  file[1].open(B.c_str());

  root_file = new TFile(C.c_str(), "RECREATE");
  for (int i = 2; i < 8; ++i)
    {
      stringstream ss;
      ss <<  "inclusive_jets_0" << i;
      string name = ss.str();
      float key = i/10.0;
      TH1F *hist = new TH1F(name.c_str(), name.c_str(), 200, 0.0, 100.0);
      jet_hists[key] = *hist;
    } 
  pions = new TH1F("pions", "pions", 200, 0.0, 100.0);
  n_events = new TH1F("N", "number of events", 1, -0.5, 0.5);

  m_pydat1.mstj(41)   = 1;
  m_pydat1.mstj(43)   = 3;
  m_pydat1.parj(82)   = 1.0;
  m_yadat1.yapars(1)  = 0.0; // f_med parameter enhancing branching kernels
  m_yadat1.yapars(2)  = 0.0; // Delta Q^2 integrated along parton path, i.e. normalization of profile.dat
  m_yadat1.yapars(3)  = 0.8; // coefficient linking YAPARS(2) and induced radiation
  m_yadat1.yapars(4)  = 0.1; // coefficient linking YAPARS(2) and drag force
  m_yadat1.yapars(10) = 0.5; // for use with YAFLAGS(3)=1, max. medium temperature corresponding to initial value in profile.dat
  m_yadat1.yaflags(1) = 0;   // additional debug info on/off (0)
  m_yadat1.yaflags(2) = 1;   // probabilistic parton formation time on/off (1)
  m_yadat1.yaflags(3) = 0;   // jet-photon conversion model on/off (0) !Experimental!

  // Set CKIN(3) to some a reasonable minimum value
  pysubs.ckin[2] = 5.0;

  m_pydatr.mrpy(1)    = _ranseed; // we will set this per job.
}

 yajem_manager::~yajem_manager()
 {
   cout << "PDG pids: " << endl;
   BOOST_FOREACH(int pdg, pids | boost::adaptors::map_keys)
     {
       cout << " " << pdg << ": " << pids[pdg] << endl;
     }
   
   root_file->Write();
 }

bool
yajem_manager::generate_event(int _event_number,
			      const trajectory_manager::trajectory_type& trajectory,
			      const interaction_manager::interaction_type& interaction)
{
  // Loop over the two partons and the two back to back trajectories.
  int kf1 = interaction.id1;
  int kf2 = interaction.id2;
  double y = interaction.y;
  double beta = interaction.beta;
  double cos_theta = interaction.cos_theta;
  //  cout << "cos_theta = " << cos_theta << endl;
  double pecm = interaction.roots;
  double pecm_2 = pecm/2.0;

  if (event_number != _event_number)
    {
      n_events->Fill(0);
      event_number = _event_number;
    }

  BOOST_FOREACH(GenEvent *eptr, raw_events)
    {
      delete eptr;
    }
  BOOST_FOREACH(GenEvent *eptr, events)
    {
      delete eptr;
    }
  raw_events.clear();
  events.clear();

  bool poorly_formed_event = false;
  IO_HEPEVT hepevtio;
  BOOST_FOREACH(TGraph *g, trajectory)
    {
      const double *yaprofilet = g->GetX();
      const double *yaprofileq = g->GetY();
      const int N = g->GetN();
      double integral = g->Integral();

      m_yadat1.yapars(2) = 1.0*integral/0.06;  // Delta Q^2
						   // integrated along
						   // parton path,
						   // i.e. normalization
						   // of profile.dat.
#if 0
      cout << integral << "," << m_yadat1.yapars(2) << endl;
#endif
      // This is code derived from yajem.f
      double qnorm = 0.0;
      for (int i = 0; i < N; ++i)
	{
	  m_yadat.yaprofilet(i+1) = fabs(yaprofilet[i]);
	  m_yadat.yaprofileq(i+1) = yaprofileq[i];
	  qnorm = qnorm + yaprofileq[i];
	}

      double qsum = 0.0;
      for (int i = 0; i < N; ++i)
	{
	  qsum = qsum + yaprofileq[i]/qnorm;
	  m_yadat.yaprofileint(i+1) = qsum;
	}

      int ip = -1;
      int ip1 = 1;
      int ip2 = 2;
      int njoin = 2;
      int ijoin[2] = {1, 2};

      py2ent_(ip, kf1, kf2, pecm);
      pyjoin_(njoin, ijoin[0]);
      pyshow_(ip1, ip2, pecm_2);
      pyexec_();

      // int listval = 1;
      // pylist_(&listval);

      // I don't think HepMC can deal with an edited PYTHIA record
      // int medit = 1;
      // pyedit_(medit);

      // We rotate to the theta* and boost to the NN center of mass frame.
      int imi = 0;
      int ima = 0;
      double the = acos(cos_theta);
      double phi = 0.0;
      double bex = 0.0;
      double bey = 0.0;
      double bez = beta;
      pyrobo_(imi,ima,the,phi,bex,bey,bez);
      
      call_pyhepc(1);
      // Create a HepMC event from the PYJETS common and set it aside
      // for now.
      GenEvent *eptr = new GenEvent(*hepevtio.read_next_event());
      eptr->set_event_number(event_number);
      raw_events.push_back(eptr);
    }

  // Get out those knives!  We're going to slice the pythia events
  // into two pieces, swap some pieces, and stitch the results back
  // together.
  identify_subevents();

  return true;
}

void
yajem_manager::identify_subevents()
{
  FourVector vec(0.0, 0.0, 0.0);
  GenVertex* vertices[2];

  // Create a couple of new HEPMC events that will hold rearranged
  // parts of the original two PYTHIA events. 
  events.push_back(new GenEvent());
  events.push_back(new GenEvent());

  int i = 0;
  BOOST_FOREACH(GenEvent * eptr, events)
    {
      eptr->set_event_number(event_number);
      // This vertex will hold all the final state particles.  I'm
      // placing them at (0,0,0), but we could propagate information
      // from the hydro code to give these hard process vertices
      // non-zero locations.
      eptr->add_vertex(new GenVertex(vec));  
      for (GenEvent::vertex_iterator vptr = eptr->vertices_begin();
	   vptr != eptr->vertices_end(); ++vptr)
	{	
	  vertices[i] = *vptr;
	}
      i++;
    }

  i = 0;
  // For the first input event, particles with momentum in the +z
  // direction go into the first output event; for the second input
  // event, they go into the second output event.  Just the opposite
  // orderings for the particles oriented along the -z direction.
  int s[2][2] = {{0, 1},{1,0}};
  int j;
  BOOST_FOREACH(GenEvent * eptr, raw_events)
    {
      list<GenParticle *> finalstateparticles;
      HepMC::copy_if(eptr->particles_begin(), eptr->particles_end(), 
		     back_inserter(finalstateparticles), IsStateFinal());
      BOOST_FOREACH(GenParticle * pptr, finalstateparticles)
	{
	  // Create a new copy of this particle.
	  GenParticle *newparticle = new GenParticle(*pptr);
	  
	  int j =  (pptr->momentum().pz() > 0.0) ? 1 : 0;
	  int idx = s[i][j];
	  newparticle->suggest_barcode(pptr->barcode());
	  vertices[idx]->add_particle_out(newparticle);
	}
      i = (i + 1) % 2;  // 0, 1, 0, 1, ...
    }
      
#if 0
  for (int i = 0; i < 2; ++i)
    {
      file[i] << *events[i];
      delete ve[i];
    }
#endif

}

void
yajem_manager::analyze_event()
{
  // Grab all the final state particles and run fastjet on them
  BOOST_FOREACH(GenEvent * eptr, events)
    {
      list<GenParticle*> finalstateparticles;
      HepMC::copy_if(eptr->particles_begin(), eptr->particles_end(), 
		     back_inserter(finalstateparticles), IsStateFinal());
      vector<PseudoJet> particles;
      BOOST_FOREACH(GenParticle * pptr, finalstateparticles)
	{
	  int pid = pptr->pdg_id();
	  pids[pid]++;
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
      BOOST_FOREACH(float R, jet_hists | boost::adaptors::map_keys)
	{
	  JetDefinition jet_def(antikt_algorithm, R);
	  ClusterSequence cs(particles, jet_def);
	  vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());	  
	  BOOST_FOREACH(PseudoJet jet, jets)
	    {
	      jet_hists[R].Fill(jet.pt());
	    }
	}
    }
}
				    
