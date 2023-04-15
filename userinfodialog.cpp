#include "userinfodialog.h"
#include "ui_userinfodialog.h"

UserInfoDialog::UserInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog)
{
    ui->setupUi(this);
    loadData();
}

UserInfoDialog::~UserInfoDialog()
{
    delete ui;
}

void UserInfoDialog::loadData()
{
    //清除旧数据
    ui->tableWidget->setRowCount(0);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("admindatabase");
    db.setUserName("kylin");
    db.setPassword("kylin");
    bool ok = db.open();
    if(ok){
        //qDebug() << "数据库链接成功";
        QSqlQuery query("select name,student_id from students_face_recognition");
        int row = 0;
        while(query.next()){
            //qDebug() << "row data:" << query.value(0).toString() << query.value(1).toString();
            QTableWidgetItem *nameItem = new QTableWidgetItem(query.value(0).toString());
            QTableWidgetItem *studentIdItem = new QTableWidgetItem(query.value(1).toString());

            QPushButton *deleteButton = new QPushButton("删除");
            QPushButton *editButton = new QPushButton("修改");

            QWidget *actionWidget = new QWidget();
            QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
            actionLayout->addWidget(deleteButton);
            actionLayout->addWidget(editButton);
            actionLayout->setAlignment(Qt::AlignCenter);
            actionLayout->setContentsMargins(0,0,0,0);
            actionWidget->setLayout(actionLayout);
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row,0,nameItem);
            ui->tableWidget->setItem(row,1,studentIdItem);
            ui->tableWidget->setCellWidget(row,2,actionWidget);

            connect(deleteButton,&QPushButton::clicked,this,&UserInfoDialog::deleteData);
            connect(editButton,&QPushButton::clicked,[this,row](){editData(row);});

            row++;
        }
        //qDebug() << "total rows:" << row;
    }else{
        qDebug() << "database connection failed:" << db.lastError().text();
    }
}

void UserInfoDialog::deleteData()
{
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    if(!senderButton){
        qDebug() << "could not determine the sender button";
        return;
    }

    QPoint buttonPos = ui->tableWidget->viewport()->mapFromGlobal(senderButton->mapToGlobal(QPoint(0,0)));
    QModelIndex index = ui->tableWidget->indexAt(buttonPos);
    if(!index.isValid()){
        qDebug() << "could not determine the table row";
        return;
    }
    int row = index.row();
    QString studentId = ui->tableWidget->item(row,1)->text();
    qDebug() << "deleting row" << row << "with studentId" << studentId;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"删除确认","确定要删除这条记录吗？",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("delete from students_face_recognition where student_id = :student_id");
        query.bindValue(":student_id",studentId);
        qDebug() << "Executing delete query:" << query.lastQuery();
        bool succuss = query.exec();
        if(succuss){
            //qDebug() << "current database:" << QSqlDatabase::database().connectionName() << QSqlDatabase::database().databaseName();
            QSqlDatabase::database().commit();
            QMessageBox::information(this,"删除成功","用户已成功删除");
            loadData();
        }else{
            QMessageBox::information(this,"删除失败","无法删除用户");
        }
    }
  }

void UserInfoDialog::editData(int row)
{
    QTableWidgetItem *nameItem = ui->tableWidget->item(row,0);
    QTableWidgetItem *studentIdItem = ui->tableWidget->item(row,1);

    UserEditDialog editDialog(this);
    editDialog.setName(nameItem->text());
    editDialog.setStudentId(studentIdItem->text());

    if(editDialog.exec() == QDialog::Accepted){
        QString newName = editDialog.getName();
        QString newStudentId = editDialog.getStudentId();
        QSqlQuery query;
        query.prepare("update students_face_recognition set name = :name,student_id = :student_id where student_id = :old_student_id");
        query.bindValue(":name",newName);
        query.bindValue(":student_id",newStudentId);
        query.bindValue(":old_student_id",studentIdItem->text());
        bool success = query.exec();
        if(success){
            nameItem->setText(newName);
            studentIdItem->setText(newStudentId);
            QMessageBox::information(this,"更新成功","用户信息已成功更新");
        }else{
            QMessageBox::warning(this,"更新失败","无法更新用户信息");
        }
    }
}


