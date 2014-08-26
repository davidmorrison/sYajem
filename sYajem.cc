#include "boost/foreach.hpp"
#include "boost/program_options.hpp" 
#include "ncoll_manager.h"
#include "trajectory_manager.h"
#include "interaction_manager.h"
#include "yajem_manager.h"

using namespace std;
namespace po = boost::program_options;

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
    ("nevents,n", po::value<int>(&nevents)->default_value(1000), 
     "number of events to generate")
    ("csv,c", po::value<bool>(&csv)->default_value(false), 
     "generate CSV output");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);   

  // Initialize YaJEM and set up output files for our events
  yajem_manager yajem("yajem");

  // This tells us how many binary collisions there are in a given event.
  ncoll_manager ncoll_generator(1164.0, 62.0);

  // Read in the back-to-back density weighted trajectories.  It is
  // assumed that these are generated with starting locations
  // distributed according to Ncoll(x,y).
  trajectory_manager trajectories("fout_jetpaths_10k_starter.root");
  
  // Read in a bunch of pre-generated parton interactions (from EPS09,
  // perhaps).
  interaction_manager interactions("partons.dat");
  
  for (int event_number = 0; event_number < nevents; ++event_number)
    {
      cout << "working on event: " << event_number << endl;
      int ncoll = ncoll_generator();
      while (ncoll > 0)
	{
	  if (yajem.generate_event(event_number,
				   trajectories.next_trajectory(),
				   interactions.next_interaction()))
	    {
	      yajem.analyze_event();
	      ncoll--;
	    }
	}
    }
  
  return 0;
}
