#include "usereditdialog.h"
#include "ui_usereditdialog.h"

UserEditDialog::UserEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserEditDialog)
{
    ui->setupUi(this);
}

UserEditDialog::~UserEditDialog()
{
    delete ui;
}

QString UserEditDialog::getName() const
{
    return ui->nameLineEdit->text();
}

void UserEditDialog::setName(const QString &name)
{
    ui->nameLineEdit->setText(name);
}

QString UserEditDialog::getStudentId() const
{
    return ui->studentIdLineEdit->text();
}

void UserEditDialog::setStudentId(const QString &studentId)
{
    ui->studentIdLineEdit->setText(studentId);
}

void UserEditDialog::on_saveButton_clicked()
{
    accept();
}

void UserEditDialog::on_cancelButton_clicked()
{
    reject();
}
