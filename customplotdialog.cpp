#include "customplotdialog.h"

CustomPlotDialog::CustomPlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomPlotDialog)
{
    ui->setupUi(this);
}

CustomPlotDialog::~CustomPlotDialog()
{
    delete ui;

    ui->Chart->setInteraction(QCP::iRangeDrag, true);
    ui->Chart->setInteraction(QCP::iRangeZoom, true);
}

void CustomPlotDialog::setData(QVector<double> y, QVector<double> x, QColor color)
{
    ui->Chart->addGraph();
    ui->Chart->graph(0)->setPen(QPen(color));
    ui->Chart->graph(0)->setData(x, y);
    ui->Chart->xAxis->setRange(0, x.size());
    ui->Chart->yAxis->setRange(-10, 10);
    ui->Chart->replot();
    return;
}
