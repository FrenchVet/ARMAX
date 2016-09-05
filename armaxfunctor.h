#ifndef ARMAXFUNCTOR_H
#define ARMAXFUNCTOR_H
#include "armaxmodel.h"

using namespace std;

class armaxfunctor: public Armaxmodel
{
public:
  armaxfunctor();
  double operator() (const dlib::matrix<double,0,1>& Model) const;
  dlib::matrix<double,0,1> y;
  dlib::matrix<double,0,1> E;
};

#endif // ARMAXFUNCTOR_H
