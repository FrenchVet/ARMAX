#ifndef OTHERFUNCTIONS_H
#define OTHERFUNCTIONS_H
#include "QDialog"
#include "dlib2/optimization.h"

typedef dlib::matrix<double,0,1> column_vector;

double autocorrelation(column_vector points, int shift);
//Eigen::MatrixXf GenerateAutocorMatrix (QVector<double> points, int order);
//Eigen::VectorXf GenerateAutocorVector (QVector<double> points, int order);
//QVector<double> ComputeModel (Eigen::VectorXf Model, QVector<double> BeginningSamples, int extention);
//Eigen::VectorXf ComputeNoiseTerms (Eigen::VectorXf model, QVector<double> points);
QVector<double> ToQVector (column_vector Input);
column_vector ToDlibVector (QVector<double> Input);

//column_vector GridSearch (Armaxmodel Model);
//column_vector GridSearch ();
#endif // OTHERFUNCTIONS_H
