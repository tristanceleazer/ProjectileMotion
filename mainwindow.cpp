#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QUrl>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);

    ui->textBrowser->setStyleSheet("QTextEdit { background-color: rgb(0, 0, 0); }");
    ui->textBrowser->setTextColor(QColorConstants::Green);
    ui->textBrowser_2->setStyleSheet("QTextEdit { background-color: rgb(0, 0, 0); }");
    ui->textBrowser_2->setTextColor(QColorConstants::Green);
    ui->textBrowser->setText("Press the 'Calculate!!!' button to start!");
    ui->textBrowser->append("> _");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
}


void MainWindow::on_radioButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_pushButton_clicked()
{
    velInit = ui->velspinBox->value();
    angle = ui->anglespinBox->value();
    height = ui->heightspinBox->value();
    steps = ui->stepspinBox->value();

    if (velInit > 0 && angle > 0 && height > 0 && steps > 0){
        calculate();
    } else if (velInit <= 0 || angle <= 0 || height <= 0 || steps <= 0){
        ui->textBrowser->setText("Please enter a valid number! (a non-negative integer.)");
        ui->textBrowser_2->setText("Please enter a valid number! (a non-negative integer.)");
        ui->textBrowser->append(">_");
        ui->textBrowser_2->append(">_");

        ui->radioButton->setChecked(true);
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(1);
    }


}

void MainWindow::calculate(){
    gravity = -9.81;

    XVel = velInit * qCos(qDegreesToRadians(angle));
    YVel = velInit * qSin(qDegreesToRadians(angle));
    ui->textBrowser->setText("Init-Vel\tX-Vel\tY-Vel");
    ui->textBrowser->append(QString::number(velInit, 'g', 4) + " m/s \t" +
                            QString::number(XVel, 'g', 4) + " m/s \t" + QString::number(YVel, 'g', 4)
                            + " m/s");

    time0 = qMax((-YVel + qSqrt(YVel * YVel - 4 * gravity * 0.5 * height)) / (2 * gravity * 0.5) ,
                 (-YVel - qSqrt(YVel * YVel - 4 * gravity * 0.5 * height)) / (2 * gravity * 0.5));

    interval = time0 / steps;

    ui->textBrowser->append(" ");
    ui->textBrowser->append("No\tTime\tX-Pos\tY-Pos");

    QStringList categories;
    QBarSet *set0 = new QBarSet("Ball Height");

    time = 0;
    x = 0;
    while(time <= time0){
        x = x + 1;
        time = time + interval;
        XPos = XVel * time;
        YPos = height + YVel * time + 0.5 * gravity * (time * time);
        if (YPos >= 0){
            ui->textBrowser->append(QString::number(x, 'g', 4) + "\t" + QString::number(time, 'g', 4) +
                                    "s \t" + QString::number(XPos, 'g', 4) + "m \t" +
                                    QString::number(YPos, 'g', 4) + "m");
            *set0 << YPos;
        } else if (YPos < 0){
            break;
        }
    }

    x = 0;
    time = 0;
    XPos = 0;
    YPos = 0;

    QBarSeries *series = new QBarSeries();
    series->append(set0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Projectile Motion Diagram");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->horizontalFrame);
    chartView->setFixedSize(ui->horizontalFrame->size());
    chartView->show();

    ui->textBrowser->append(" ");
    ui->textBrowser->append("The ball hit the ground at " + QString::number(time0, 'g', 4) +
                            " seconds after starting");
    ui->textBrowser->append("Time Interval (Time between steps): " + QString::number(interval, 'g', 4)
                            + "s");
    ui->textBrowser->append("> _");

    finalXPos = XVel * time0;
    ui->textBrowser_2->setText("Final Position: " + QString::number(finalXPos, 'g', 4) +
                               "m From Start Line");
    ui->textBrowser_2->append("Final time (y=0): " + QString::number(time0, 'g', 4) + "s");
    ui->textBrowser_2->append("Velocity: X-Axis = " + QString::number(XVel, 'g', 4) +
                              "m/s; Y-Axis = " + QString::number(YVel, 'g', 4) + "m/s");

}


void MainWindow::on_actionOnline_Help_triggered()
{
    QDesktopServices::openUrl(QUrl("https://tristanceleazer.github.io/projects/projectilemotion/projectilemotion.html", QUrl::TolerantMode));
}

