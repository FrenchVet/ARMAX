#ifndef ARMAXMAINWINDOW_H
#define ARMAXMAINWINDOW_H

#include <QMainWindow>
#include "ui_armaxmainwindow.h"
#include <iostream>
#include <math.h>
#include "other_functions.h"
#include <vector>
#include "csvparser.h"
#include "armaxsolver.h"
#include "armaxmodel.h"
#include <fstream>
#include <sstream>
#include "dlib2/optimization.h"
#include "customplotdialog.h"

typedef dlib::matrix<double,0,1> column_vector;

namespace Ui {
class ARMAXmainWindow;
}

class ARMAXmainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ARMAXmainWindow(QWidget *parent = 0);
    ~ARMAXmainWindow();

public slots:

    void LoadEpsilonsFromFile(){this->on_actionLoad_epsilons_triggered();}
    void Fit(){;}
    void aproximateCurrentModel(){;}
    void PlotEpsilons();
    void ComputeEpsilons();
    void ComputeACFdata();
    void ComputepACFdata(){;}
    void ComputeACFepsilons();
    void ComputepACFepsilons(){;}
    void DecreaseARModel();
    void IncreaseARModel();
    void DecreaseMAModel();
    void IncreaseMAModel();
    void ChangeMAModel(QTableWidgetItem* item);
    void ChangeARModel(QTableWidgetItem* item);
    void ControlField();

    //ToDO: progress bar, separate thread, ultra resizable graphics,


private slots:
    void on_actionLoad_data_triggered();

    void on_actionLoad_epsilons_triggered();

    void on_actionSave_model_triggered();

    void on_actionClear_memory_triggered();

private:
    Ui::ARMAXmainWindow *ui;
    Armaxsolver Solver;
    QVector<double> y;
    QVector<double> E;
    QVector<double> AR;
    QVector<double> MA;
};

#endif // ARMAXMAINWINDOW_H
