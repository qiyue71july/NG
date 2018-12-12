#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(int *tag, QWidget *parent) :
    QDialog(parent) , m_ptag(tag) , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    if(!tag)
        exit(0);
    connect(ui->m_qradiobutton_develop,SIGNAL(clicked(bool)),this,SLOT(slotClickedRadio(bool)));
    connect(ui->m_qradiobutton_enginer,SIGNAL(clicked(bool)),this,SLOT(slotClickedRadio(bool)));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::slotClickedRadio(bool status)
{
    QString qstr_sendername = sender()->objectName();
    if(ui->m_qradiobutton_develop->objectName() == qstr_sendername)
        *this->m_ptag = 0;
    else if(ui->m_qradiobutton_enginer->objectName() == qstr_sendername)
        *this->m_ptag = 1;
    this->close();
}
