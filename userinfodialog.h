#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QPushButton>
#include <QSqlQuery>
#include <databaseutils.h>
#include <QMessageBox>
#include <QHBoxLayout>
#include <usereditdialog.h>

namespace Ui {
class UserInfoDialog;
}

class UserInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoDialog(QWidget *parent = nullptr);
    ~UserInfoDialog();

private:
    Ui::UserInfoDialog *ui;
    DatabaseUtils *dataUtils;
    void loadData();
    void deleteData();
    void editData(int row);
};

#endif // USERINFODIALOG_H
