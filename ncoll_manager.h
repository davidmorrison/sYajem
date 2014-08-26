#ifndef __NCOLL_MANAGER_H__
#define __NCOLL_MANAGER_H__

#include "TRandom.h"

class ncoll_manager
{
 public:
  ncoll_manager(double _mean, double _sigma) 
    : mean(_mean), sigma(_sigma)
  {
    generator = new TRandom();
  }
  
  double operator()(){ return generator->Gaus(mean,sigma);}

  TRandom *generator;
  double mean;
  double sigma;
};

#endif /* __NCOLL_MANAGER_H__ */
