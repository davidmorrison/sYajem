#ifndef __INTERACTION_MANAGER_H__
#define __INTERACTION_MANAGER_H__

#include <vector>
#include <string>

struct
interaction_manager
{
  struct interaction_type
  {
    int id1;
    int id2;
    double x1;
    double x2;
    double roots;
    double Q;
    double y;
    double beta;
    double cos_theta;
  };
  
  interaction_manager(const std::string &file);
  interaction_type next_interaction();
  
  std::vector<interaction_type> interactions;
  std::vector<interaction_type>::iterator iptr;
};

#endif /* __INTERACTION_MANAGER_H__ */
