#include "fittingthread.h"

FittingThread::FittingThread(QObject *parent) : QThread(parent)
{
    this->Solver = new Armaxsolver();
}

FittingThread::~FittingThread()
{
}

void FittingThread::SetSolvingCriteria(bool UseCurrentEpsilons, bool UseStartingPoints, column_vector StartingPoints)
{
    this->UseCurrentEpsilons = UseCurrentEpsilons;
    this->UseStartingPoints = UseStartingPoints;
    this->StartingPoints = StartingPoints;
    return;
}

void FittingThread::run()
{
    if(this->Solver->ReturnMA().size() == 0)
    {
        this->Solver->FitJustAR();
    }
    else
    {
        if(this->UseCurrentEpsilons)
        {

            if(this->UseStartingPoints)
            {
                this->Solver->FitModelwithEpsilons(this->StartingPoints);
            }
            else
            {
                this->Solver->FitModelwithEpsilons();
            }
        }
        else
        {
            if(this->UseStartingPoints)
            {
                this->Solver->FitModel(this->StartingPoints);
            }
            else
            {
                this->Solver->FitModel();
            }
        }
    }
    emit FittingComplete();

 return;
}
