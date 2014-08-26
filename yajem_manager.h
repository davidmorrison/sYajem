#ifndef __YAJEM_MANAGER_H__
#define __YAJEM_MANAGER_H__

#include <string>
#include <fstream>
#include "trajectory_manager.h"
#include "interaction_manager.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include "HepMC/GenEvent.h"
#include "TFile.h"
#include "TH1F.h"

struct yajem_manager
{
  yajem_manager(const std::string& _root = "yajem", int _ranseed = 11035493);
  ~yajem_manager();

  
  bool generate_event(int event_number,
		      const trajectory_manager::trajectory_type& trajectory, 
		      const interaction_manager::interaction_type& interaction);

  void identify_subevents();
  void analyze_event();

  int event_number;
  std::string root;
  int ranseed;

  std::ofstream file[2]; 
  
  TFile *root_file;
  boost::ptr_map<float, TH1F> jet_hists;
  boost::ptr_map<int, TH1F> particle_hists;
  std::map<int,int> pids;
  TH1 *pions;
  TH1 *n_events;
  // These hold pointers to events generated for each binary collision
  std::vector<HepMC::GenEvent *> raw_events;
  std::vector<HepMC::GenEvent *> events;
};

#endif /* __YAJEM_MANAGER_H__ */
