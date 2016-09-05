#include "armaxmodel.h"

/////////////////////////////////////////////CONSTRUCTORS////////////////////////////////////////////////////

Armaxmodel::Armaxmodel()
{

    this->ARMA.set_size(2);
    this->ARMA = 0, 0;
}


//////////////////////////////////////////SETTING VALUES//////////////////////////////////////////////

void Armaxmodel::SetSignal(column_vector signal)
{
  this->y = signal;
}

void Armaxmodel::SetAutoRegression(column_vector AutoRegression)
{
  this->AR = AutoRegression;
  this->ARMA(0) = AutoRegression.size();
}

void Armaxmodel::SetMovingAverage(column_vector MovingAverage)
{
    this->MA = MovingAverage;
    this->ARMA(1) = MovingAverage.size();
}

void Armaxmodel::SetEpsilons(column_vector Epsilons)
{
    this->E = Epsilons;
    this->ComputeSigma();
    this->ComputeAIC();
    this->ComputeFPE();
}

///////////////////////////////////////////////////RETURNING//////////////////////////////////////

column_vector  Armaxmodel::ReturnAutoRegressionParameters()
{
  return this->AR;
}

column_vector  Armaxmodel::ReturnMovingAveregeParameters()
{
  return this->MA;
}

column_vector  Armaxmodel::ReturnSignal()
{
  return this->y;
}

column_vector  Armaxmodel::ReturnAproximation()
{
  return this->yAprox;
}


column_vector  Armaxmodel::ReturnPredictions()
{
  return this->yPredicted;
}

column_vector  Armaxmodel::ReturnEpsilons()
{
  return this->E;
}


double Armaxmodel::ReturnSigma()
{
  return this->Sigma;
}

double Armaxmodel::ReturnAIC()
{
  return this->AIC;
}

double Armaxmodel::ReturnFPE()
{
  return this->FPE;
}

int Armaxmodel::ReturnSumOfParameterts()
{
    return dlib::sum(this->ARMA);
}

////////////////////////////////////////////////SOLVING/////////////////////////////////////

void Armaxmodel::ComputeEpsilons(column_vector HighOrderAR)
{
    if(this->y.size()< HighOrderAR.size())
    {
        return;
    }
    else
    {
        this->E.set_size(this->y.size());
        int Count = 0;
        for(Count; Count < HighOrderAR.size(); Count++)
        {
            this->E(Count) = this->y(Count);
        }
        for(Count; Count < this->y.size(); Count++)
        {
            this->E(Count) = this->y(Count);
            for(int m = 0; m < HighOrderAR.size(); m++)
            {
                this->E(Count) -= this->y(Count-m-1)*HighOrderAR(m);
            }
        }
    }

    this->ComputeSigma();
    this->ComputeAIC();
    this->ComputeFPE();
    return;
}

 dlib::matrix<double,0,0> Armaxmodel::ComputeACMatrix(int Size)
 {
     if ((this->y.size())/2 > Size)
     {
         dlib::matrix<double,0,0> Output;
         Output.set_size(Size, Size);
         for (int i = 0; i < Size; i++) //first element was equal to 1, rest need to be calculated
             {
               Output(0, i) = autocorrelation(this->y, i);
             }


           for (int rows = 1; rows < Size; rows++)
             {
               int columns = rows;//+1;
               for (columns; columns < Size; columns++)
                 {
                  Output(rows, columns) = Output(rows-1, columns-1);
                 }
               for (columns = 0; columns < rows; columns++)
                  {
                   Output(rows, columns)= Output(columns, rows);
                  }

             }
         return Output;

     }
     else
     {
         column_vector nothing;
         return nothing;
     }
 }


column_vector Armaxmodel::ComputeACVector(int Size)
{
    if ((this->y.size())/2 > Size)
    {
        column_vector Output;
        Output.set_size(Size);
        for (int i = 0; i < Size; i++)
        {
            Output(i) = autocorrelation(this->y, i+1);
        }
        return Output;
    }
    else
    {
        column_vector nothing;
        return nothing;
    }


}

void Armaxmodel::AproximateModel()
{
    this->yAprox.set_size(this->y.size());
    int i = 0;
    for(i; i < dlib::max(this->ARMA); i++)
    {
        this->yAprox(i) = this->E(i);
    }
    for(i; i < this->y.size(); i++)
    {
        this->yAprox(i) = this->E(i);
        for(int m = 0; m < this->ARMA(0); m++)
        {
            this->yAprox(i) += AR(m)*this->yAprox(i-m-1);
        }
        for(int m = 0; m < this->ARMA(1); m++)
        {
            this->yAprox(i) += MA(m)*this->E(i-m-1);
        }
    }
}


double Armaxmodel::operator() (const column_vector& Model) const
{
   std::vector<double> PreviousValues;
   int length = this->y.size();
   double output  = 0;
   double prediction;
   int i;
   int m;
   //cout << "Proba: " << Model(0) << " " << Model(1) << " "<< Model(2) << " "<< Model(3) << " "<< '\n';
   for(i = 0; i < dlib::max(this->ARMA); i++)
   {
       PreviousValues.push_back(this->E(i));
   }

   for(i; i < length; i++)
   {
      prediction = this->E(i);
      for(m = 0; m < this->ARMA(0); m++)
      {
          prediction += Model(m)*PreviousValues[(int)this->ARMA(0)-m-1];
      }
      for(m; m < (this->ARMA(0)+this->ARMA(1)); m++)
      {
          prediction += Model(m)*this->E(i-m+((int)this->ARMA(0))-1);
      }
      PreviousValues.erase(PreviousValues.begin());
      PreviousValues.push_back(prediction);
      output += fabs((prediction-this->y(i))/this->y(i));
   }

   output = output/length;
   return output;

}

column_vector Armaxmodel::ComputeACF(int Length)
{
    if(Length > this->y.size())
    {
        Length = this->y.size();
    }

    column_vector Output;
    Output.set_size(Length);

    for(int i = 0; i < Length; i++)
    {
        Output(i) = autocorrelation(this->y, i);
    }
    return Output;
}

column_vector Armaxmodel::ComputeACFepsilons(int Length)
{
    if(Length > this->E.size())
    {
        Length = this->E.size();
    }

    column_vector Output;
    Output.set_size(Length);

    for(int i = 0; i < Length; i++)
    {
        Output(i) = autocorrelation(this->E, i);
    }
    return Output;
}

void Armaxmodel::ComputeSigma()
{
    double mean = dlib::mean(this->E);
    this->Sigma = 0;
    for(int i = 0; i < this->E.size(); i++)
    {
        this->Sigma += pow((this->y(i) - mean),2.0);
    }
    this->Sigma = this->Sigma/(double)this->y.size();
    this->Sigma = sqrt(this->Sigma);
    return;
}

void Armaxmodel::ComputeAIC()
{
    this-> AIC = log(this->Sigma*(1.0 + 2*(double)dlib::sum(ARMA)/(double)this->y.size()));
    return;
}

void Armaxmodel::ComputeFPE()
{
    this->FPE = this->Sigma*((1.0 + (double)dlib::sum(ARMA)/(double)this->y.size())/(1.0 + (double)dlib::sum(ARMA)/(double)this->y.size()));
    return;
}

