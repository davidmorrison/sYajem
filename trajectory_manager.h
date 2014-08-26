#ifndef __TRAJECTORY_MANAGER_H__
#define __TRAJECTORY_MANAGER_H__

#include <vector>
#include "TGraph.h"
class TFile;

struct trajectory_manager
{
  typedef std::vector<TGraph*> trajectory_type;
  
  trajectory_manager(const std::string& file);
  trajectory_type next_trajectory();

  std::vector<trajectory_type> trajectories;
  std::vector<trajectory_type>::iterator iptr;
  TFile *file;
};


#endif /* __TRAJECTORY_MANAGER_H__ */
