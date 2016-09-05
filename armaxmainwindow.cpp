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

    ui->Chart->addGraph();
    ui->Chart->graph(0)->setData(x, this->y);
    ui->Chart->xAxis->setRange(0, x.size());
    ui->Chart->yAxis->setRange(-10, 10);
    ui->Chart->replot();

    this->Solver.SetInputData(ToDlibVector(this->y));

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
    this->Solver.SetEpsilons(ToDlibVector(this->E));
    return;
}

void ARMAXmainWindow::on_actionSave_model_triggered()
{

}

void ARMAXmainWindow::on_actionClear_memory_triggered()
{

}

void ARMAXmainWindow::ComputeEpsilons()
{
    this->Solver.ComputeEpsilons(ui->HighOrderARedit->text().toInt());
    this->PlotEpsilons();
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

void ARMAXmainWindow::ChangeMAModel(QTableWidgetItem* item)
{
    QString text = item->text();
    bool isNumber;
    double newNumber = text.toDouble(&isNumber);
    if(!isNumber)
    {
        item->setText("0");
    }
    QString numberCorrection;
    numberCorrection.setNum(newNumber);
    item->setText(numberCorrection);
    MA[item->row()] = newNumber;

    this->Solver.SetMA(ToDlibVector(this->MA));

    return;
}

void ARMAXmainWindow::ChangeARModel(QTableWidgetItem* item)
{
    QString text = item->text();
    bool isNumber;
    double newNumber = text.toDouble(&isNumber);
    if(!isNumber)
    {
        item->setText("0");
    }
    QString numberCorrection;
    numberCorrection.setNum(newNumber);
    item->setText(numberCorrection);
    AR[item->row()] = newNumber;
    this->Solver.SetAR(ToDlibVector(this->AR));
    return;
}

void ARMAXmainWindow::IncreaseMAModel()
{
    ui->MATable->insertRow(ui->MATable->rowCount());
    this->MA.resize(ui->MATable->rowCount());
    QTableWidgetItem *newItem = ui->MATable->itemAt(0,0)->clone();
    newItem->setText("0");
    ui->MATable->setItem((ui->MATable->rowCount()-1),0, newItem);
    this->Solver.SetMA(ToDlibVector(this->MA));
    return;
}

void ARMAXmainWindow::IncreaseARModel()
{
    ui->ARTable->insertRow(ui->ARTable->rowCount());
    this->AR.resize(ui->ARTable->rowCount());
    QTableWidgetItem *newItem = ui->ARTable->itemAt(0,0)->clone();
    newItem->setText("0");
    ui->ARTable->setItem((ui->ARTable->rowCount()-1),0, newItem);
    this->Solver.SetAR(ToDlibVector(this->AR));
    return;
}

void ARMAXmainWindow::DecreaseMAModel()
{
    ui->MATable->removeRow(ui->MATable->rowCount()-1);
    this->MA.pop_back();
    return;
}

void ARMAXmainWindow::DecreaseARModel()
{
    ui->ARTable->removeRow(ui->ARTable->rowCount()-1);
    this->AR.pop_back();
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
    text = ui->pACFdataLengthBox->text();
    if (~text.isEmpty()){
        try
        {
            int number = text.toInt();
            if(number <= 0)
            {
                number = 0;
            }
            ui->pACFdataLengthBox->setText(QString::number(number));

        }
        catch (int e)
        {
            ui->pACFdataLengthBox->setText(QString::number(0));
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
    text = ui->pACFepsilonsLengthBox->text();
    if (~text.isEmpty()){
        try
        {
            int number = text.toInt();
            if(number <= 0)
            {
                number = 0;
            }
            ui->pACFepsilonsLengthBox->setText(QString::number(number));

        }
        catch (int e)
        {
            ui->pACFepsilonsLengthBox->setText(QString::number(0));
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
