#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <databaseutils.h>
#include <QPushButton>
#include <userdialog.h>
#include <userinfodialog.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <vector>
#include <cstdio>

using namespace cv;

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
    void on_login_pushButton_clicked();

    void on_recoginize_PushButton_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseUtils *datautils;
    QSerialPort *serial;
    QTimer *timer;
};
#endif // MAINWINDOW_H
