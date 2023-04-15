#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
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
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <userdialog.h>
#include <vector>
#include <cstdio>
using namespace cv;

namespace Ui {
class UserDialog;
}

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = nullptr);
    ~UserDialog();

private slots:
    void displayFrame();

    void on_back_pushButton_clicked();
    void onSerialDataReceived();
    void onTimerTimeout();

    void openCamera();

    void openSerial();

    void on_Select_PushButton_clicked();

private:
    Ui::UserDialog *ui;
    cv::VideoCapture capture;
    cv::Mat frame;
    QLabel *face_label;
    QTimer *timer;
    QSerialPort *serial;
    enum class TempMode{Body,Object};
    TempMode currentMode;
};

#endif // USERDIALOG_H


