#include "armaxmainwindow.h"


typedef dlib::matrix<double,0,1> column_vector;

using namespace std;

ARMAXmainWindow::ARMAXmainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ARMAXmainWindow)
{
    ui->setupUi(this);
    this->AR.push_back(0);
    this->MA.push_back(0);

    ui->Chart->setInteraction(QCP::iRangeDrag, true);
    ui->Chart->setInteraction(QCP::iRangeZoom, true);

    this->Thread = new FittingThread();
}


ARMAXmainWindow::~ARMAXmainWindow()
{
    delete ui;
}

void ARMAXmainWindow::on_actionLoad_data_triggered()
{

    this->y.clear();

    QString fileName;
    QVector<double> x;
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"C://", tr("Text files (*.txt *.csv)"));
    const char* CharFileName = fileName.toStdString().c_str();
    CsvParser *csvparserDane = CsvParser_new(CharFileName, ",", 0);
    CsvRow *rzadDane;
    int i = 0;
    while (rzadDane = CsvParser_getRow(csvparserDane))
    {
        const char **rowFields = CsvParser_getFields(rzadDane);
        this->y.push_back(stod(rowFields[0]));
        x.push_back(i);
        i++;
    }

    CsvParser_destroy(csvparserDane);

    if(this->y.size() > 0)
    {

        ui->Chart->addGraph();
        ui->Chart->graph(0)->setData(x, this->y);
        ui->Chart->xAxis->setRange(0, x.size());
        ui->Chart->yAxis->setRange(-10, 10);
        ui->Chart->replot();


        this->Solver.SetInputData(ToDlibVector(this->y));


        ui->ACFdataButton->setEnabled(true);
        ui->ACFdataLengthBox->setEnabled(true);
        ui->ComputeEpsilonsButton->setEnabled(true);
        ui->HighOrderARedit->setEnabled(true);

        ui->FitButton->setEnabled(true);
        ui->UseStartingPointCheckbox->setEnabled(true);


        if(this->MA.size() == 0)
        {
            ui->AproximationButton->setEnabled(true);
        }
    }
    else
    {
        ui->Chart->clearGraphs();
        ui->Chart->replot();
        ui->ACFdataButton->setEnabled(false);
        ui->ACFdataLengthBox->setEnabled(false);
        ui->ComputeEpsilonsButton->setEnabled(false);
        ui->HighOrderARedit->setEnabled(false);

        ui->FitButton->setEnabled(false);
        ui->UseStartingPointCheckbox->setEnabled(false);
        ui->UseStartingPointCheckbox->setChecked(false);
        ui->UseEpsilonsCheckbox->setEnabled(false);
        ui->UseEpsilonsCheckbox->setChecked(false);

        ui->AproximationButton->setEnabled(false);

        ui->ACFepsilonsButton_2->setEnabled(false);
        ui->ACFepsilonsLengthBox->setEnabled(false);
        ui->plotEpsilonsButton->setEnabled(false);

    }
    return;
}

void ARMAXmainWindow::on_actionLoad_epsilons_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"C://", tr("Text files (*.txt *.csv)"));
    const char* CharFileName = fileName.toStdString().c_str();
    CsvParser *csvparserDane = CsvParser_new(CharFileName, ",", 0);
    CsvRow *rzadDane;
    while (rzadDane = CsvParser_getRow(csvparserDane))
    {
        const char **rowFields = CsvParser_getFields(rzadDane);
        this->E.push_back(stod(rowFields[0]));
    }

    CsvParser_destroy(csvparserDane);
    if(this->y.size() > 0 && this->E.size() > 0)
    {
        this->Solver.SetEpsilons(ToDlibVector(this->E));
        this->PlotEpsilons();


        this->Solver.ComputeSigma();
        this->Solver.ComputeAICFPE();
        this->UpdateSigmaLabels();

        ui->ACFepsilonsButton_2->setEnabled(true);
        ui->ACFepsilonsLengthBox->setEnabled(true);
        ui->plotEpsilonsButton->setEnabled(true);

        ui->AproximationButton->setEnabled(true);
        ui->UseEpsilonsCheckbox->setEnabled(true);
    }

    return;
}

void ARMAXmainWindow::on_actionSave_model_triggered()
{

    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,tr("Save Model"),"C://", tr("Text files (*.txt *.csv)"));
    std::ofstream file;
    file.open(fileName.toStdString());
    file << "AR" << "," << "MA\n";
    int length = max(this->AR.size(), this->MA.size());
    for (int i = 0; i < length; i++)
    {
        if(i < this->AR.size())
        {
            file << this->AR[i] << ",";
        }
        else
        {
            file << "0,";
        }
        if(i < this->MA.size())
        {
            file << this->MA[i] << "\n";
        }
        else
        {
            file << "0\n";
        }
    }
    file.close();
}

void ARMAXmainWindow::on_actionClear_memory_triggered()
{
    ui->ACFdataButton->setEnabled(false);
    ui->ACFdataLengthBox->setEnabled(false);
    ui->ComputeEpsilonsButton->setEnabled(false);
    ui->HighOrderARedit->setEnabled(false);

    ui->FitButton->setEnabled(false);
    ui->UseStartingPointCheckbox->setEnabled(false);
    ui->UseStartingPointCheckbox->setChecked(false);
    ui->UseEpsilonsCheckbox->setEnabled(false);
    ui->UseEpsilonsCheckbox->setChecked(false);

    ui->AproximationButton->setEnabled(false);

    ui->ACFepsilonsButton_2->setEnabled(false);
    ui->ACFepsilonsLengthBox->setEnabled(false);
    ui->plotEpsilonsButton->setEnabled(false);
    this->y.clear();
    this->E.clear();
    this->AR.clear();
    this->AR.resize(1);
    this->AR[0] = 0;
    this->MA.clear();
    this->MA.resize(1);
    this->MA[0] = 0;


    ui->MATable->setRowCount(1);
    QTableWidgetItem *newItem = new QTableWidgetItem();
    newItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    newItem->setText("0");
    ui->MATable->setItem(0,0, newItem);

    ui->ARTable->setRowCount(1);
    QTableWidgetItem *newItem2 = new QTableWidgetItem();
    newItem2->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    newItem2->setText("0");
    ui->ARTable->setItem(0,0, newItem2);

//    for(int i = 0; i < ui->Chart->graphCount(); i++)
//    {
//        ui->Chart->graph(i)->clearData();
//        ui->Chart
//    }
    ui->Chart->clearGraphs();
    ui->Chart->replot();
    return;

}

void ARMAXmainWindow::aproximateCurrentModel()
{
    column_vector Output = this->Solver.ReturnAproximation();
    this->Solver.SetEpsilons(ToDlibVector(this->E));
    this->Solver.ComputeSigma();
    this->Solver.ComputeAICFPE();
    this->UpdateSigmaLabels();
    double MSE = 0;
    QVector<double> yAprox = ToQVector(Output);
    QVector<double> x(yAprox.size());
    for(int k = 0; k < yAprox.size(); k++)

    {
        x[k] = k;
        MSE += pow((this->y[k] - yAprox[k]),2);

    }

    MSE = sqrt(MSE/(double)yAprox.size());
    QString label = "RMSE: " + QString::number(MSE);
    ui->MSElabel->setText(label);
    ui->Chart->addGraph();
    ui->Chart->graph(1)->setPen(QPen(QColor(255, 100, 0)));
    ui->Chart->graph(1)->setData(x, yAprox);
    ui->Chart->replot();
    ui->statusBar->showMessage(QString::fromStdString("Model aproximated!"));
    return;

}

void ARMAXmainWindow::modelFited()
{
    this->E = ToQVector(this->Thread->Solver->ReturnEpsilons());
    this->Solver.SetEpsilons(this->Thread->Solver->ReturnEpsilons());
    this->AR = ToQVector(this->Thread->Solver->ReturnAR());
    this->MA = ToQVector(this->Thread->Solver->ReturnMA());
    this->Solver.SetMA(this->Thread->Solver->ReturnMA());
    this->Solver.SetAR(this->Thread->Solver->ReturnAR());
    this->Solver.ComputeSigma();
    this->Solver.ComputeAICFPE();
    this->UpdateSigmaLabels();
    this->UpdateARMAtable();
    column_vector Output = this->Thread->Solver->ReturnAproximation();
    QVector<double> yAprox = ToQVector(Output);
    double MSE = 0;
    QVector<double> x(yAprox.size());
    for(int k = 0; k < yAprox.size(); k++)
    {
        x[k] = k;
        MSE += pow((this->y[k] - yAprox[k]),2);
    }
    MSE = sqrt(MSE/(double)yAprox.size());
    QString label = "RMSE: " + QString::number(MSE);
    ui->MSElabel->setText(label);
    ui->Chart->addGraph();
    ui->Chart->graph(1)->setPen(QPen(QColor(255, 100, 0)));
    ui->Chart->graph(1)->setData(x, yAprox);

    ui->Chart->replot();

    ui->ACFepsilonsButton_2->setEnabled(true);
    ui->ACFepsilonsLengthBox->setEnabled(true);
    ui->plotEpsilonsButton->setEnabled(true);

    ui->AproximationButton->setEnabled(true);
    ui->UseEpsilonsCheckbox->setEnabled(true);
    ui->statusBar->showMessage(QString::fromStdString("Fit complete!"));
    return;

}

void ARMAXmainWindow::Fit()
{
    this->Thread->terminate();
    ui->statusBar->showMessage(QString::fromStdString("Processing..."));
    QObject::connect(Thread, SIGNAL(FittingComplete()), this, SLOT(modelFited()));

    this->Thread->Solver->SetInputData(this->Solver.ReturnSignal());
    this->Thread->Solver->SetAR(ToDlibVector(this->AR));
    this->Thread->Solver->SetMA(ToDlibVector(this->MA));

    if(ui->UseEpsilonsCheckbox->isChecked())
    {
        this->E = ToQVector(this->Solver.ReturnEpsilons());
        if(ui->UseStartingPointCheckbox->isChecked())
        {
            column_vector StartingPoints;
            StartingPoints.set_size(this->AR.size() + this->MA.size());
            int i;
            for(i = 0; i < this->AR.size(); i++)
            {
                StartingPoints(i) = this->AR[i];
            }
            for(i; i < StartingPoints.size(); i++)
            {
                StartingPoints(i) = this->MA[i-this->AR.size()];
            }
            this->Thread->SetSolvingCriteria(ui->UseEpsilonsCheckbox->isChecked(),ui->UseStartingPointCheckbox->isChecked(),StartingPoints);
        }
        else
        {
            column_vector StartingPoints;
            this->Thread->SetSolvingCriteria(ui->UseEpsilonsCheckbox->isChecked(),ui->UseStartingPointCheckbox->isChecked(),StartingPoints);
        }
    }
    else
    {
        if(ui->UseStartingPointCheckbox->isChecked())
        {
            column_vector StartingPoints;
            StartingPoints.set_size(this->AR.size() + this->MA.size());
            int i;
            for(i = 0; i < this->AR.size(); i++)
            {
                StartingPoints(i) = this->AR[i];
            }
            for(i; i < StartingPoints.size(); i++)
            {
                StartingPoints(i) = this->MA[i-this->AR.size()];
            }
            this->Thread->SetSolvingCriteria(ui->UseEpsilonsCheckbox->isChecked(),ui->UseStartingPointCheckbox->isChecked(),StartingPoints);
        }
        else
        {
            column_vector StartingPoints;
            this->Thread->SetSolvingCriteria(ui->UseEpsilonsCheckbox->isChecked(),ui->UseStartingPointCheckbox->isChecked(),StartingPoints);
        }
    }
    this->Thread->start();
    return;
}

void ARMAXmainWindow::ComputeEpsilons()
{
    this->E = ToQVector(this->Solver.ComputeEpsilons(ui->HighOrderARedit->text().toInt()));
    this->PlotEpsilons();
    this->Solver.ComputeSigma();
    this->Solver.ComputeAICFPE();
    this->UpdateSigmaLabels();
    if(this->y.size() > 0 && this->E.size() > 0)
    {
        ui->ACFepsilonsButton_2->setEnabled(true);
        ui->ACFepsilonsLengthBox->setEnabled(true);
        ui->plotEpsilonsButton->setEnabled(true);

        ui->AproximationButton->setEnabled(true);
        ui->UseEpsilonsCheckbox->setEnabled(true);
    }
    return;
}

void ARMAXmainWindow::PlotEpsilons()
{
    CustomPlotDialog *dial = new CustomPlotDialog();
    QVector<double> x(this->E.size());
    for(int i = 0; i < this->E.size(); i++)
    {
        x[i] = i;
    }
    dial->setData(this->E, x, QColor(200, 100, 100));
    dial->show();
    return;
}

void ARMAXmainWindow::ComputeACFdata()
{
    int length = ui->ACFdataLengthBox->text().toInt();
    CustomPlotDialog *dial = new CustomPlotDialog();
    QVector<double> x(length);
    for(int i = 0; i < length; i++)
    {
        x[i] = i;
    }
    QVector<double> y(length);
    y = ToQVector(this->Solver.ReturnACF(length));
    dial->setData(y, x, QColor(200, 100, 100));
    dial->show();
    return;
}

void ARMAXmainWindow::ComputeACFepsilons()
{
    int length = ui->ACFdataLengthBox->text().toInt();
    CustomPlotDialog *dial = new CustomPlotDialog();
    QVector<double> x(length);
    for(int i = 0; i < length; i++)
    {
        x[i] = i;
    }
    QVector<double> y(length);
    y = ToQVector(this->Solver.ReturnACFepsilons(length));
    dial->setData(y, x, QColor(200, 100, 100));
    dial->show();
    return;
}


void ARMAXmainWindow::IncreaseMAModel()
{
    if(this->MA.size() < 10)
    {
        ui->MATable->insertRow(ui->MATable->rowCount());
        this->MA.resize(ui->MATable->rowCount());
        QTableWidgetItem *newItem = new QTableWidgetItem();
        newItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        newItem->setText("0");
        ui->MATable->setItem((ui->MATable->rowCount()-1),0, newItem);
        this->Solver.SetMA(ToDlibVector(this->MA));
        this->UpdateARMAname();
        if(this->E.size() > 0)
        {
            this->Solver.ComputeAICFPE();
            this->UpdateSigmaLabels();
        }
        if(this->E.size() == 0 && this->MA.size() > 0)
        {
            ui->AproximationButton->setEnabled(false);
        }
    }
    return;
}

void ARMAXmainWindow::IncreaseARModel()
{
    if(this->AR.size() < 10)
    {
        ui->ARTable->insertRow(ui->ARTable->rowCount());
        this->AR.resize(ui->ARTable->rowCount());
        QTableWidgetItem *newItem = new QTableWidgetItem();
        newItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        newItem->setText("0");
        ui->ARTable->setItem((ui->ARTable->rowCount()-1),0, newItem);
        this->Solver.SetAR(ToDlibVector(this->AR));
        this->UpdateARMAname();
        if(this->E.size() > 0)
        {
            this->Solver.ComputeAICFPE();
            this->UpdateSigmaLabels();
        }
    }
    return;
}

void ARMAXmainWindow::DecreaseMAModel()
{
    if(this->MA.size() > 0 && !(this->MA.size() == 1 &&  this->AR.size() == 0))
    {
        ui->MATable->removeRow(ui->MATable->rowCount()-1);
        this->MA.pop_back();
        this->UpdateARMAname();
        if(this->E.size() > 0)
        {
            this->Solver.ComputeAICFPE();
            this->UpdateSigmaLabels();
        }
        if(this->E.size() == 0 && this->MA.size() == 0 && this->y.size() > 0)
        {
            ui->AproximationButton->setEnabled(true);
        }
    }
    return;
}

void ARMAXmainWindow::DecreaseARModel()
{
    if(this->AR.size() > 0 && !(this->AR.size() == 1 &&  this->MA.size() == 0))
    {
        ui->ARTable->removeRow(ui->ARTable->rowCount()-1);
        this->AR.pop_back();
        this->UpdateARMAname();
        if(this->E.size() > 0)
        {
            this->Solver.ComputeAICFPE();
            this->UpdateSigmaLabels();
        }
    }
    return;
}

void ARMAXmainWindow::ControlField() //CHANGE TO SIGNAL MAPPING (but it worksa now so for now it is fine)
{
    QString text = ui->ACFdataLengthBox->text();
    if (~text.isEmpty()){
        try
        {
            int number = text.toInt();
            if(number <= 0)
            {
                number = 0;
            }
            ui->ACFdataLengthBox->setText(QString::number(number));

        }
        catch (int e)
        {
            ui->ACFdataLengthBox->setText(QString::number(0));
        }
    }

    text = ui->ACFepsilonsLengthBox->text();
    if (~text.isEmpty()){
        try
        {
            int number = text.toInt();
            if(number <= 0)
            {
                number = 0;
            }
            ui->ACFepsilonsLengthBox->setText(QString::number(number));

        }
        catch (int e)
        {
            ui->ACFepsilonsLengthBox->setText(QString::number(0));
        }
    }

    text = ui->HighOrderARedit->text();
    if (~text.isEmpty()){
        try
        {
            int number = text.toInt();
            if(number <= 0)
            {
                number = 0;
            }
            ui->HighOrderARedit->setText(QString::number(number));

        }
        catch (int e)
        {
            ui->HighOrderARedit->setText(QString::number(0));
        }
    }
}

void ARMAXmainWindow::UpdateARMAname()
{
   if(this->AR.size() > 0 && this->MA.size() > 0 )
   {
   QString Title = "ARMA(" + QString::number(this->AR.size()) + ", " + QString::number(this->MA.size()) + ")";
   ui->Model->title().clear();
   ui->Model->setTitle(Title);
   return;
   }
   else
   {
       if(this->AR.size() > 0)
       {
           QString Title = "AR(" + QString::number(this->AR.size()) + ")";
           ui->Model->title().clear();
           ui->Model->setTitle(Title);
           return;
       }
       else
       {
           QString Title = "MA(" + QString::number(this->MA.size()) + ")";
           ui->Model->title().clear();
           ui->Model->setTitle(Title);
           return;
       }
   }
}

void ARMAXmainWindow::UpdateSigmaLabels()
{
    QString Label = "sigma: " + QString::number(this->Solver.ReturnSigma());
    ui->SigmaLabel->setText(Label);
    Label = "AIC: " + QString::number(this->Solver.ReturnAIC());
    ui->AIClabel->setText(Label);
    Label = "FPE: " + QString::number(this->Solver.ReturnFPE());
    ui->FPClabel->setText(Label);
}

void ARMAXmainWindow::UpdateARMAtable()
{

    for(int i = 0; i < this->AR.size(); i++)
    {

        ui->ARTable->item(i,0)->setText(QString::number(this->AR[i]));
    }

    for(int i = 0; i < this->MA.size(); i++)
    {
        ui->MATable->item(i,0)->setText(QString::number(this->MA[i]));
    }

    return;
}

void ARMAXmainWindow::CheckTable()
{
    for(int i = 0; i < ui->ARTable->rowCount(); i++)
    {
        QString text = ui->ARTable->item(i,0)->text();
        bool isNumber;
        double newNumber = text.toDouble(&isNumber);
        if(!isNumber)
        {
            ui->ARTable->item(i,0)->setText("0");
        }
        else
        {
            QString numberCorrection;
            numberCorrection.setNum(newNumber);
            ui->ARTable->item(i,0)->setText(numberCorrection);
        }
        AR[i] = newNumber;
        this->Solver.SetAR(ToDlibVector(this->AR));
    }
    std::cout << "\n";
    for(int i = 0; i < ui->MATable->rowCount(); i++)
    {
        QString text = ui->MATable->item(i,0)->text();
        bool isNumber;
        double newNumber = text.toDouble(&isNumber);
        if(!isNumber)
        {
            ui->MATable->item(i,0)->setText("0");
        }
        QString numberCorrection;
        numberCorrection.setNum(newNumber);
        ui->MATable->item(i,0)->setText(numberCorrection);
        MA[i] = newNumber;
        this->Solver.SetMA(ToDlibVector(this->MA));
    }
    return;
}
