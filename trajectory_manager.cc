#include <cstdio>
#include <vector>
#include "trajectory_manager.h"
#include "TGraph.h"
#include "TFile.h"

using namespace std;

trajectory_manager::trajectory_manager(const string& filename)
{
  file = new TFile(filename.c_str());
  char name0[20], name1[20];
  TGraph *g0, *g1;
  for (int i = 0; i < 10000; ++i)
    {
      sprintf(name0, "parton_0_%d", i);
      sprintf(name1, "parton_1_%d", i);

      TGraph *g0 = (TGraph *)file->Get(name0);
      TGraph *g1 = (TGraph *)file->Get(name1);
      trajectory_type v;
      v.push_back(g0);
      v.push_back(g1);
      trajectories.push_back(v);
    }
  iptr = trajectories.begin();
}

trajectory_manager::trajectory_type
trajectory_manager::next_trajectory()
{
  trajectory_manager::trajectory_type temp = *iptr;
  iptr++;
  if (iptr == trajectories.end())
    {
      iptr = trajectories.begin();
    }
    
  return temp;
}

      
