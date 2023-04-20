#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

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
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent = nullptr);
    ~AddUserDialog();

private slots:
    void on_cancel_PushButton_clicked();
    void openCamera();
    void displayFrame();

private:
    Ui::AddUserDialog *ui;
    cv::VideoCapture capture;
    cv::Mat frame;
    QLabel *face_label;
    QTimer *timer;
};

#endif // ADDUSERDIALOG_H
