#ifndef ARMAXMODEL_H
#define ARMAXMODEL_H
#include <iostream>
#include <vector>
#include <cmath>
#include <QMainWindow>
#include "dlib/optimization.h"
#include <stdlib.h>
#include "other_functions.h"

typedef dlib::matrix<double,0,1> column_vector;

class Armaxmodel
{
public:
  Armaxmodel();
  int ReturnSumOfParameterts();

  void SetSignal(column_vector  signal);
  void SetAutoRegression(column_vector  AutoRegression);
  void SetMovingAverage(column_vector  MovingAverage);
  void SetEpsilons(column_vector  Epsilons);

  column_vector  ReturnAutoRegressionParameters();
  column_vector  ReturnMovingAveregeParameters();
  column_vector  ReturnSignal();
  column_vector  ReturnAproximation();
  column_vector  ReturnPredictions();
  column_vector  ReturnEpsilons();

  double ReturnSigma();
  double ReturnAIC();
  double ReturnFPE();

  column_vector  ComputeACF(int Length);
  column_vector  ComputeACFepsilons(int Length);

  void ComputeSigma();
  void CalculateAICFPE();

  void ComputeEpsilons(column_vector HighOrderAR);
  dlib::matrix<double,0,0> ComputeACMatrix(int Size);
  column_vector ComputeACVector(int Size);
  void AproximateModel();

  double operator() (const column_vector& Model) const;

private:


  column_vector  AR;
  column_vector  MA;
  dlib::matrix<int,0,1> ARMA;
  column_vector y;
  column_vector yAprox;
  column_vector yPredicted;
  column_vector E;

  double Sigma = 1;
  double AIC = 0;
  double FPE = 0;
};

#endif // ARMAXMODEL_H
