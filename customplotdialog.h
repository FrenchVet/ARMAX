#ifndef CUSTOMPLOTDIALOG_H
#define CUSTOMPLOTDIALOG_H

#include <QDialog>
#include "qcustomplot.h"
#include "ui_customplotdialog.h"
//#include "customplotdialog.ui"

namespace Ui {
class CustomPlotDialog;
}

class CustomPlotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomPlotDialog(QWidget *parent = 0);
    ~CustomPlotDialog();
    void setData(QVector<double> y, QVector<double> x, QColor color);

    Ui::CustomPlotDialog *ui;
};

#endif // CUSTOMPLOTDIALOG_H
