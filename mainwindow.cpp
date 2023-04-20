#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    datautils->connectDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_login_pushButton_clicked()
{
    QString username = ui->name_lineEdit->text();
    QString password = ui->pwd_lineEdit->text();
    if(datautils->is_checked(username,password)){
        qInfo() << "\033[32m登录成功\033[0m" ;
        UserInfoDialog *userInfoDialog = new UserInfoDialog(this);
        userInfoDialog->exec();
    }else{
        QMessageBox::warning(this,"登录提示","登录失败");
        qCritical() << "登录失败" ;
    }
}

void MainWindow::on_recoginize_PushButton_clicked()
{
    UserDialog *userDialog = new UserDialog(this);
    userDialog->exec();
}
