#include "colorcopydialog.h"
#include "ui_colorcopydialog.h"
#include <QMessageBox>
#include <QDebug>

ColorCopyDialog::ColorCopyDialog(QMap<int, AlarmColor> &map, QWidget *parent) : QDialog(parent),
    ui(new Ui::ColorCopyDialog),m_qmap_alarmcolor(map)
{
    ui->setupUi(this);
    this->m_qmap_alarmcolor_current = this->m_qmap_alarmcolor;
    initView();
    initComboboxLevel();
    connect(ui->m_qcombobox_source,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangedCombobox(int)));
    connect(ui->m_qpushbutton_enter,SIGNAL(clicked(bool)),this,SLOT(slotClickedButton(bool)));
    connect(ui->m_qpushbutton_cancel,SIGNAL(clicked(bool)),this,SLOT(slotClickedButton(bool)));
}

ColorCopyDialog::~ColorCopyDialog()
{
    delete ui;
}

void ColorCopyDialog::initView() const
{
    ui->m_qlistwidget_destination->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->m_qlistwidget_destination->setSelectionMode(QAbstractItemView::MultiSelection);
}

void ColorCopyDialog::initComboboxLevel() const
{
    QMap<int,AlarmColor>::const_iterator iterator_color = this->m_qmap_alarmcolor_current.begin();
    while (this->m_qmap_alarmcolor_current.end() != iterator_color)
    {
        ui->m_qcombobox_source->insertItem(iterator_color.value().m_nalarm_level,iterator_color.value().m_qstr_alarm_name);
        ++iterator_color;
    }
    ui->m_qcombobox_source->setCurrentIndex(-1);
    ui->m_qlistwidget_destination->clear();
    return;
}

void ColorCopyDialog::initListWidgetDestination(QString source)
{
    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_alarmcolor_current.begin();
    while (this->m_qmap_alarmcolor_current.end() != iterator_color)
    {
        if (source != iterator_color.value().m_qstr_alarm_name)
        {
            ui->m_qlistwidget_destination->addItem(iterator_color.value().m_qstr_alarm_name);
        }
        ++iterator_color;
    }
    return;
}

AlarmColor *ColorCopyDialog::getSourceColor()
{
    QString qstr_levelname = ui->m_qcombobox_source->currentText();
    if (qstr_levelname.isEmpty())
    {
        return NULL;
    }

    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_alarmcolor_current.begin();
    while (this->m_qmap_alarmcolor_current.end() != iterator_color)
    {
        if (qstr_levelname == iterator_color.value().m_qstr_alarm_name)
        {
            return &this->m_qmap_alarmcolor_current[iterator_color.key()];
        }
        ++iterator_color;
    }
    return NULL;
}

AlarmColor *ColorCopyDialog::getDestinationColor(QString text)
{
    if (text.isEmpty())
    {
        return NULL;
    }

    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_alarmcolor_current.begin();
    while (this->m_qmap_alarmcolor_current.end() != iterator_color)
    {
        if (text == iterator_color.value().m_qstr_alarm_name)
        {
            return &this->m_qmap_alarmcolor_current[iterator_color.key()];
        }
        ++iterator_color;
    }
    return NULL;
}

void ColorCopyDialog::slotChangedCombobox(int index)
{
    Q_UNUSED(index);
    QString qstr_sendername = sender()->objectName();
    if (ui->m_qcombobox_source->objectName() == qstr_sendername)
    {
        ui->m_qlistwidget_destination->clear();
        initListWidgetDestination(ui->m_qcombobox_source->currentText());
    }
    return;
}

void ColorCopyDialog::slotClickedButton(bool status)
{
    Q_UNUSED(status);
    QString qstr_sendername = sender()->objectName();
    if (ui->m_qpushbutton_enter->objectName() == qstr_sendername)
    {
        if (ui->m_qcombobox_source->currentText().isEmpty())
        {
            return;
        }

        QList<QListWidgetItem*> qlist_items = ui->m_qlistwidget_destination->selectedItems();
        if (qlist_items.isEmpty())
        {
            return;
        }

        AlarmColor* palarmcolor_temp = NULL;
        QList<QListWidgetItem*>::iterator iterator_items = qlist_items.begin();
        while (qlist_items.end() != iterator_items)
        {
            palarmcolor_temp = getDestinationColor((*iterator_items)->text());
            if (palarmcolor_temp)
            {
                palarmcolor_temp->m_qstr_alarm_text.operator =(getSourceColor()->m_qstr_alarm_text);
                palarmcolor_temp->m_qstr_alarm_back.operator =(getSourceColor()->m_qstr_alarm_back);
                palarmcolor_temp->m_qstr_alarm_flash_text.operator =(getSourceColor()->m_qstr_alarm_flash_text);
                palarmcolor_temp->m_qstr_alarm_flash_back.operator =(getSourceColor()->m_qstr_alarm_flash_back);
                palarmcolor_temp->m_qstr_recover_text.operator =(getSourceColor()->m_qstr_recover_text);
                palarmcolor_temp->m_qstr_recover_back.operator =(getSourceColor()->m_qstr_recover_back);
                palarmcolor_temp->m_qstr_recover_flash_text.operator =(getSourceColor()->m_qstr_recover_flash_text);
                palarmcolor_temp->m_qstr_recover_flash_back.operator =(getSourceColor()->m_qstr_recover_flash_back);
            }
            ++iterator_items;
            palarmcolor_temp = NULL;
        }

        m_qmap_alarmcolor = m_qmap_alarmcolor_current;
        QMessageBox::information(this,"复制信息","复制完成！");
        ui->m_qlistwidget_destination->clearSelection();
    }
    else if (ui->m_qpushbutton_cancel->objectName().operator ==(qstr_sendername.toLatin1()))
    {
        this->close();
    }
    return;
}
