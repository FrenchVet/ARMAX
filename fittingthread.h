#ifndef FITTINGTHREAD_H
#define FITTINGTHREAD_H

#include <QObject>
#include "armaxsolver.h"
#include <QThread>
#include "dlib/optimization.h"

typedef dlib::matrix<double,0,1> column_vector;

class FittingThread : public QThread
{
    Q_OBJECT
public:
    FittingThread(QObject *parent = 0);
    Armaxsolver * Solver;
     ~FittingThread();
    void SetSolvingCriteria(bool UseCurrentEpsilons, bool UseStartingPoints, column_vector StartingPoints);

signals:
    void FittingComplete();

protected:
    void run();

private:
    bool UseCurrentEpsilons;
    bool UseStartingPoints;
    column_vector StartingPoints;

public slots:
};

#endif // FITTINGTHREAD_H
