#include <fstream>
#include <sstream>
#include <string>
#include "interaction_manager.h"

using namespace std;

interaction_manager::interaction_manager(const std::string &filename) 
{
  ifstream file(filename.c_str());
  string line;
  while (getline(file,line))
    {
      stringstream ss(line);
      interaction_manager::interaction_type t;
      ss  >> t.id1
	  >> t.id2
	  >> t.x1
	  >> t.x2
	  >> t.roots
	  >> t.Q
	  >> t.y 
	  >> t.beta
	  >> t.cos_theta;
      interactions.push_back(t);
    }
  file.close();
  iptr = interactions.begin();
}

interaction_manager::interaction_type
interaction_manager::next_interaction()
{
  interaction_manager::interaction_type temp = *iptr;
  iptr++;
  if (iptr == interactions.end())
    {
      iptr = interactions.begin();
    }
    
  return temp;
}
