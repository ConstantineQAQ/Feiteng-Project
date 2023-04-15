#ifndef USEREDITDIALOG_H
#define USEREDITDIALOG_H

#include <QDialog>

namespace Ui {
class UserEditDialog;
}

class UserEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserEditDialog(QWidget *parent = nullptr);
    ~UserEditDialog();

    QString getName() const;
    void setName(const QString &name);

    QString getStudentId() const;
    void setStudentId(const QString &studentId);

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::UserEditDialog *ui;
};

#endif // USEREDITDIALOG_H
