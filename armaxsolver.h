#ifndef ARMAXSOLVER_H
#define ARMAXSOLVER_H
#include "armaxmodel.h"
#include "other_functions.h"
typedef dlib::matrix<double,0,1> column_vector;

class Armaxsolver
{
public:
  Armaxsolver();

  void SetAR (column_vector AutoReg);
  void SetMA (column_vector MovingAvg);
  void SetInputData(column_vector Data);
  void SetEpsilons(column_vector Epsilons);

  column_vector ReturnACF(int length);
  column_vector ReturnACFepsilons(int length);
  column_vector ReturnSignal();
  column_vector ReturnEpsilons();
  column_vector ReturnAproximation();
  double ReturnSigma();
  double ReturnMSEofFit();
  column_vector ReturnAR();
  column_vector ReturnMA();
  double ReturnAIC();
  double ReturnFPE();

  void ComputeSigma();
  void ComputeAICFPE();

  void FitJustAR();
  void FitModel(); //full auto
  void FitModelwithEpsilons(); //set epsilons
  void FitModel(column_vector StartPoints);
  void FitModelwithEpsilons(column_vector StartPoints);

  column_vector ComputeEpsilons(int HighOrderArmaLevel);

private:

  column_vector GridSearch ();
  void ApplySolution(column_vector Solution);
  Armaxmodel Model;
  dlib::matrix<int,0,1> ARMA;
  double MSE = 0;

};

#endif // ARMAXSOLVER_H
