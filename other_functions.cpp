#include "other_functions.h";

QVector<double> ToQVector (column_vector Input)
{
   QVector<double> Output(Input.size());
   for(int i = 0; i < Input.size(); i++)
   {
       Output[i] = Input(i);
   }
   return Output;
}

column_vector ToDlibVector (QVector<double> Input)
{
    int length = Input.length();
    column_vector Output;
    Output.set_size(length);
    for(int i = 0; i < length; i++)
    {
        Output(i) = Input[i];
    }
    return Output;
}

double autocorrelation(column_vector points, int shift)
{
  double outcome = 0;
  int i;
  for (i = 0; i < (points.size()-shift); i++)
    {
      outcome += points(i) * points(i+shift);
    }
  outcome = outcome/points.size();
  return outcome;
}

//Eigen::VectorXf ComputeNoiseTerms (Eigen::VectorXf model, QVector<double> points)
//{
//  Eigen::VectorXf output(points.size());
//  int i;
//  for (i = 0; i < model.rows(); i++)
//    {
//      output(i) = points[i];
//    }
//  for (i; i < points.size(); i++)
//    {
//      output(i) = points[i];
//      for (int m = 0; m < model.rows(); m++)
//        {
//          output(i) -= points[i-m-1]*model(m);
//        }
//    }
//  return output;
//}

//Eigen::MatrixXf GenerateAutocorMatrix (QVector<double> points, int order)
//{
//  Eigen::MatrixXf output(order, order);

////  output(0,0) = 1.0; //first row calculation - every coeficient is in the matrix

//  for (int i = 0; i < order; i++) //first element was equal to 1, rest need to be calculated
//    {
//      output(0, i) = autocorrelation(points, i);
//    }


//  for (int rows = 1; rows < order; rows++)
//    {
//   //   output(rows, rows) = 1.0;
//      int columns = rows;//+1;
//      for (columns; columns < order; columns++)
//        {
//         output(rows, columns) = output(rows-1, columns-1);
//        }
//      for (columns = 0; columns < rows; columns++)
//         {
//          output(rows, columns)= output(columns, rows);
//         }

//    }

//  return output;
//}

//Eigen::VectorXf GenerateAutocorVector (QVector<double> points, int order)
//{
//  Eigen::VectorXf output(order);
//  for (int i = 0; i < order; i++)
//    {
//      output(i) = autocorrelation(points, i+1);
//    }
//  return output;
//}

//QVector<double> ComputeModel (Eigen::VectorXf Model, QVector<double> BeginningSamples, int extention)
//{
//  QVector<double> output;

//  for (int k = 0; k < (Model.rows()); k++)
//    {
//      output.append(BeginningSamples[k]);
//    }

//  for (int i = 0; i < extention; i++)
//    {
//      double value = 0;
//      for (int k = 0; k < (Model.rows()); k++)
//        {
//          value += output[output.length()-k-1]*Model(k);
//        }
//      output.append(value);
//    }
//  return output;
//}
