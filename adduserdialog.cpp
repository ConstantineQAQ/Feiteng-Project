#include "adduserdialog.h"
#include "ui_adduserdialog.h"

AddUserDialog::AddUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDialog)
{
    ui->setupUi(this);
    openCamera();
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

void AddUserDialog::on_cancel_PushButton_clicked()
{
    capture.release();
    timer->stop();
    delete timer;
    qInfo() <<"我被释放了";
    this->close();
}

void AddUserDialog::displayFrame()
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

void AddUserDialog::openCamera()
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
