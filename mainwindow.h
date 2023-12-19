#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_pushButton_clicked();

    void on_actionOnline_Help_triggered();

private:
    Ui::MainWindow *ui;

    long double XVel, YVel, velInit, angle, height, XPos, YPos, gravity, x, steps, time0, time, interval, finalXPos;

    void calculate();
};
#endif // MAINWINDOW_H
