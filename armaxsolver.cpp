#include "armaxsolver.h"

////////////////////////////////////////////////CONSTRUCTORS/////////////////////////////////////////




Armaxsolver::Armaxsolver()
{

    this->ARMA.set_size(2);
    this->ARMA = 0, 0;
}

void Armaxsolver::SetInputData(column_vector Data)
{
    this->Model.SetSignal(Data);
    return;
}

void Armaxsolver::SetAR(column_vector AutoReg)
{
    this->ARMA(0) = AutoReg.size();
    this->Model.SetAutoRegression(AutoReg);
    return;
}


void Armaxsolver::SetMA(column_vector MovingAvg)
{
    this->ARMA(1) = MovingAvg.size();
    this->Model.SetMovingAverage(MovingAvg);
    return;
}

void Armaxsolver::SetEpsilons(column_vector Epsilons)
{
    this->Model.SetEpsilons(Epsilons);
    return;
}

////////////////////////////////////////////////RETURNING/////////////////////////////////////////

column_vector Armaxsolver::ReturnSignal()
{
   return this->Model.ReturnSignal();
}

column_vector Armaxsolver::ReturnAproximation()
{
//    dlib::matrix<double>  AutocorMatrix = this->Model.ComputeACMatrix(10);
//    column_vector  AutocorVector = this->Model.ComputeACVector(10);

//    column_vector OutputModel;

//    AutocorMatrix = dlib::inv(AutocorMatrix);

//    OutputModel = AutocorMatrix*AutocorVector;

//    this->Model.ComputeEpsilons(OutputModel);
   this->Model.AproximateModel();
   return this->Model.ReturnAproximation();
}

column_vector Armaxsolver::ReturnACF(int length)
{
   return this->Model.ComputeACF(length);
}

column_vector Armaxsolver::ReturnACFepsilons(int length)
{
   return this->Model.ComputeACFepsilons(length);
}

column_vector Armaxsolver::ReturnAR()
{
   return this->Model.ReturnAutoRegressionParameters();
}

column_vector Armaxsolver::ReturnMA()
{
   return this->Model.ReturnMovingAveregeParameters();
}

double Armaxsolver::ReturnSigma()
{
   return this->Model.ReturnSigma();
}

double Armaxsolver::ReturnAIC()
{
   return this->Model.ReturnAIC();
}

double Armaxsolver::ReturnFPE()
{
   return this->Model.ReturnFPE();
}

double Armaxsolver::ReturnMSEofFit()
{
   return this->MSE;
}

/////////////////////////////////////////SOLVING////////////////////////////////////////////////////
column_vector Armaxsolver::ComputeEpsilons(int HighOrderArmaLevel)
{
    if(HighOrderArmaLevel == 0)
    {
        HighOrderArmaLevel = dlib::sum(this->ARMA)*5;
    }


    dlib::matrix<double>  AutocorMatrix = this->Model.ComputeACMatrix(HighOrderArmaLevel);
    column_vector  AutocorVector = this->Model.ComputeACVector(HighOrderArmaLevel);

    column_vector OutputModel;

    AutocorMatrix = dlib::inv(AutocorMatrix);

    OutputModel = AutocorMatrix*AutocorVector;

    this->Model.ComputeEpsilons(OutputModel);
    return this->Model.ReturnEpsilons();
}

void Armaxsolver::FitModel(int HighOrderArmaLevel, column_vector StartPoints)
{
    if(HighOrderArmaLevel == 0)
    {
        HighOrderArmaLevel = dlib::sum(this->ARMA)*5;
    }


    dlib::matrix<double>  AutocorMatrix = this->Model.ComputeACMatrix(HighOrderArmaLevel);
    column_vector  AutocorVector = this->Model.ComputeACVector(HighOrderArmaLevel);

    column_vector OutputModel;

    AutocorMatrix = dlib::inv(AutocorMatrix);

    OutputModel = AutocorMatrix*AutocorVector;

    this->Model.ComputeEpsilons(OutputModel);


   dlib::find_min_bobyqa(this->Model, StartPoints, (2*dlib::sum(this->ARMA)+1),dlib::uniform_matrix<double>(sum(this->ARMA),1, -1), dlib::uniform_matrix<double>(sum(this->ARMA),1, 1), 0.0001, 1e-7, 10000);
   this->ApplySolution(StartPoints);
   return;
}


void Armaxsolver::FitModel( column_vector StartPoints)
{
   this->Model.SetEpsilons(this->Model.ReturnEpsilons());
   dlib::find_min_bobyqa(this->Model, StartPoints, (2*dlib::sum(this->ARMA)+1),dlib::uniform_matrix<double>(sum(this->ARMA),1, -1), dlib::uniform_matrix<double>(sum(this->ARMA),1, 1), 0.0001, 1e-7, 10000);
   this->ApplySolution(StartPoints);
   return;
}

void Armaxsolver::FitModelwithEpsilons()
{
   this->Model.SetEpsilons(this->Model.ReturnEpsilons());

   column_vector StartingPoints;
   StartingPoints.set_size(dlib::sum(this->ARMA));
   for (int k = 0; k < dlib::sum(this->ARMA); k++)
   {
   StartingPoints(k) = 0.5;
   }
   StartingPoints = this->GridSearch();
   std::cout << StartingPoints << "\n\n";
   dlib::find_min_bobyqa(this->Model, StartingPoints, (2*dlib::sum(this->ARMA)+1),dlib::uniform_matrix<double>(sum(this->ARMA),1, -1), dlib::uniform_matrix<double>(sum(this->ARMA),1, 1), 0.0001, 1e-7, 10000);
   std::cout << StartingPoints << "\n";
   this->ApplySolution(StartingPoints);
   return;

}

void Armaxsolver::FitModel()
{
   dlib::matrix<double>  AutocorMatrix = this->Model.ComputeACMatrix(dlib::sum(this->ARMA)*5);
   column_vector  AutocorVector = this->Model.ComputeACVector(dlib::sum(this->ARMA)*5);

   column_vector OutputModel;

   AutocorMatrix = dlib::inv(AutocorMatrix);

   OutputModel = AutocorMatrix*AutocorVector;

   this->Model.ComputeEpsilons(OutputModel);

   column_vector StartingPoints;
   StartingPoints.set_size(dlib::sum(this->ARMA));
   for (int k = 0; k < dlib::sum(this->ARMA); k++)
   {
   StartingPoints(k) = 0.5;
   }
   StartingPoints = this->GridSearch();
   std::cout << StartingPoints << "\n\n";
   dlib::find_min_bobyqa(this->Model, StartingPoints, (2*dlib::sum(this->ARMA)+1),dlib::uniform_matrix<double>(sum(this->ARMA),1, -1), dlib::uniform_matrix<double>(sum(this->ARMA),1, 1), 0.0001, 1e-7, 10000);
   std::cout << StartingPoints << "\n";
   this->ApplySolution(StartingPoints);
   return;

}

void Armaxsolver::FitModel(int HighOrderArmaLevel)
{
   dlib::matrix<double>  AutocorMatrix = this->Model.ComputeACMatrix(HighOrderArmaLevel);
   column_vector  AutocorVector = this->Model.ComputeACVector(HighOrderArmaLevel);

   column_vector OutputModel;

   AutocorMatrix = dlib::inv(AutocorMatrix);

   OutputModel = AutocorMatrix*AutocorVector;

   this->Model.ComputeEpsilons(OutputModel);

   column_vector StartingPoints;
   StartingPoints.set_size(dlib::sum(this->ARMA));
   for (int k = 0; k < dlib::sum(this->ARMA); k++)
   {
   StartingPoints(k) = 0.5;
   }
   StartingPoints = this->GridSearch();
   std::cout << StartingPoints << "\n\n";
   dlib::find_min_bobyqa(this->Model, StartingPoints, (2*dlib::sum(this->ARMA)+1),dlib::uniform_matrix<double>(sum(this->ARMA),1, -1), dlib::uniform_matrix<double>(sum(this->ARMA),1, 1), 0.0001, 1e-7, 10000);
   std::cout << StartingPoints << "\n";
   this->ApplySolution(StartingPoints);
   return;

}

column_vector Armaxsolver::GridSearch ()
{
    int depth = this->Model.ReturnSumOfParameterts();
    int Max = 4;
    double minimum = 1000;

    column_vector StartingPoints;
    StartingPoints.set_size(depth);
    column_vector TempPoints;
    TempPoints.set_size(depth);

    std::vector<double> Points(Max);
    for (int i = 0; i < Max; i++)
    {
        Points[i] = -0.8 + i*0.5;
    }

    std::vector<int> Slots(depth);

    for (int i = 0; i < depth; i++)
    {
        Slots[i] = 0;
    }

    int index = 0;
    while (true)
    {

        for(int i = 0; i < depth; i++)
        {
            TempPoints(i) = Points[Slots[i]];
        }

        double error = this->Model(TempPoints);
        if(error < minimum)
        {
            StartingPoints = TempPoints;
            minimum = error;
        }


        // Increment


        Slots[0]++;


        // Carry
        while (Slots[index] == Max)
        {
            // Overflow, we're done
            if (index == depth - 1)
            {

                return StartingPoints;
            }

            Slots[index] = 0;
            index++;
            Slots[index]++;
        }

        index = 0;
    }

    return StartingPoints;

}

void Armaxsolver::ApplySolution(column_vector Solution)
{
    column_vector AR;
    AR.set_size(this->ARMA(0));
    int m;
    for(m = 0; m < this->ARMA(0); m++)
    {
        AR(m) = Solution(m);
    }
    this->SetAR(AR);
    column_vector MA;
    MA.set_size(this->ARMA(1));
    for(m; m < dlib::sum(this->ARMA); m++)
    {
        MA(m-this->ARMA(0)) = Solution(m);
    }
    this->SetMA(MA);
}
