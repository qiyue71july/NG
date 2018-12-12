#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(int * tag,QWidget *parent = 0);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    int *m_ptag = NULL;

private slots:
    void slotClickedRadio(bool status);
};

#endif // LOGINDIALOG_H
