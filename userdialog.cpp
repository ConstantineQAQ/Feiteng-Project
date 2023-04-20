#include "userdialog.h"
#include "ui_userdialog.h"

int flag = 0;

UserDialog::UserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);

    openCamera();

    openSerial();
}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::displayFrame()
{
    CascadeClassifier face_cascade;
    face_cascade.load("/home/kylin/opencv-4.6/opencv-4.6.0/data/haarcascades/haarcascade_frontalface_alt2.xml");
    if(face_cascade.empty()){
        qInfo() << "获取失败" ;
    }
    //Read frame
    cv::Mat frame;
    capture >> frame;
    //cv::cvtColor(frame,gray_frame,cv::COLOR_BGR2RGB);

    //Detect faces
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(frame,faces,1.2,3,0|cv::CASCADE_SCALE_IMAGE,cv::Size(90,90));

    //Draw faces
    for(size_t i=0;i<faces.size();i++){
        cv::rectangle(frame,faces[i],cv::Scalar(255,0,0),2);
        cv::putText(frame,"Test_test",cv::Point(faces[i].x,faces[i].y),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(0,255,0));
    }


    QImage qimage = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
    ui->face_label->setPixmap(QPixmap::fromImage(qimage));
}

void UserDialog::on_back_pushButton_clicked()
{
    capture.release();
    serial->close();
    serial->clear();
    timer->stop();
    delete timer;
    qInfo() <<"我被释放了";
    this->close();
}

void UserDialog::onSerialDataReceived()
{
    QByteArray data = serial->readAll();
    qDebug() << "raw data:" << data.toHex();

    if(data.size() >=9 && data.at(0) == static_cast<char>(0xFE)){
            if(data.at(1) == static_cast<char>(0xAC) && currentMode == TempMode::Body){
                int bodyTempInt = static_cast<unsigned char>(data.at(2));
                int bodyTempDec = static_cast<unsigned char>(data.at(3));
                double bodyTemp = bodyTempInt + bodyTempDec * 0.01;
                qDebug() << "Body temperature:" << bodyTemp << "C";
                ui->temp_label->setText(QString("人体温度 %1C").arg(bodyTemp,0,'f',1));
            }
        else if(data.at(1) == static_cast<char>(0xAA) && currentMode == TempMode::Object){
                int objectTempInt = static_cast<unsigned char>(data.at(2));
                int objectTempDec = static_cast<unsigned char>(data.at(3));
                double objectTemp = objectTempInt + objectTempDec * 0.01;
                qDebug() << "Object temperature:" << objectTemp << "C";
                ui->temp_label->setText(QString("物体温度 %1C").arg(objectTemp,0,'f',1));
        }
    }
}

void UserDialog::onTimerTimeout()
{
    QByteArray startMeasuringCmd = QByteArray::fromHex("FACAC4");
    serial->write(startMeasuringCmd);
}

void UserDialog::openCamera()
{
    face_label = new QLabel(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(displayFrame()));
    timer->start(30);

    capture.open(-1);
    if(capture.isOpened()){
        timer->start(20);
        qInfo() << "\033[32m摄像头打开成功\033[0m" ;
    }
    else{
        qCritical() << "摄像头打开失败" ;
        QMessageBox::warning(this,"摄像头连接情况","连接失败");
    }
}

void UserDialog::openSerial()
{
    serial = new QSerialPort(this);
    currentMode = TempMode::Body;
    QSerialPortInfo info;
    foreach(const QSerialPortInfo &serialPortInfo,QSerialPortInfo::availablePorts()){
        if(serialPortInfo.portName().contains("ttyUSB0")){
            info = serialPortInfo;
            break;
        }
    }
    serial->setPort(info);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(!serial->open(QIODevice::ReadWrite)){
        qCritical() << "串口打开失败";
    }

    timer = new QTimer(this);
    timer->setInterval(1000);

    connect(serial,&QSerialPort::readyRead,this,&UserDialog::onSerialDataReceived);
    connect(timer,&QTimer::timeout,this,&UserDialog::onTimerTimeout);

    QByteArray bodyTempModeCmd = QByteArray::fromHex("FAC5BF");
    serial->write(bodyTempModeCmd);

    timer->start();
}

void UserDialog::on_Select_PushButton_clicked()
{
    timer->stop();
    QByteArray startMeasuringCmd = QByteArray::fromHex("FACAC4");

    if(currentMode == TempMode::Body){
        currentMode = TempMode::Object;
        QByteArray objectTempModeCmd = QByteArray::fromHex("FAC6C0");
        serial->write(objectTempModeCmd);
        QThread::msleep(200);
        serial->write(startMeasuringCmd);
        //ui->Select_PushButton->setText("切换到体温模式");
    }else{
        currentMode = TempMode::Body;
        QByteArray bodyTempModeCmd = QByteArray::fromHex("FAC5BF");
        serial->write(bodyTempModeCmd);
        QThread::msleep(200);
        serial->write(startMeasuringCmd);
        //ui->Select_PushButton->setText("切换到物温模式");
    }
    timer->start();
}
