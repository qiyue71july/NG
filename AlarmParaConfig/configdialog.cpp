#include "configdialog.h"
#include "ui_configdialog.h"
#include <QDebug>
#include <QPalette>
#include <QColor>
#include <QColorDialog>
#include <QVariant>
#include <QFont>


ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    this->m_qfontdialog_window = new QFontDialog(this);
    this->m_qfontdialog_window ->setOption(QFontDialog::FontDialogOption::NoButtons);
    ui->m_qgroupbox_font->layout()->addWidget(this->m_qfontdialog_window);

    LoginDialog *logindialog_object = new LoginDialog(&this->m_nlogin_tag);
    logindialog_object->exec();
    if(0 == this->m_nlogin_tag)
    {
        initData();
        initManagerView();
        ui->m_qtabwidget_config->setTabEnabled(0,true);
        ui->m_qtabwidget_config->setTabEnabled(1,false);
        ui->m_qtabwidget_config->setTabEnabled(2,false);
    }
    else if(1 == this->m_nlogin_tag)
    {
        initData();
        initNodeView();
        initColorView();
        ui->m_qtabwidget_config->setTabEnabled(0,false);
        ui->m_qtabwidget_config->setTabEnabled(1,true);
        ui->m_qtabwidget_config->setTabEnabled(2,true);
    }
    else
    {
        ui->m_qtabwidget_config->setTabEnabled(0,false);
        ui->m_qtabwidget_config->setTabEnabled(1,false);
        ui->m_qtabwidget_config->setTabEnabled(2,false);
    }

    connect(ui->m_qtablewidget_node->verticalHeader(),SIGNAL(sectionDoubleClicked(int)),this,SLOT(slotDoubleClickedHeader(int)));
    connect(ui->m_qtablewidget_node->verticalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotClickedHeader(int)));
    connect(ui->m_qtablewidget_node,SIGNAL(itemSelectionChanged()),this,SLOT(slotChangedSelectionItems()));
    connect(ui->m_qtablewidget_node,SIGNAL(cellChanged(int,int)),this,SLOT(slotClickedItem(int,int)));
    connect(ui->m_qtablewidget_field,SIGNAL(cellChanged(int,int)),this,SLOT(slotClickedItem(int,int)));
    connect(ui->m_qpushbutton_node_add,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonManager(bool)));
    connect(ui->m_qpushbutton_node_sub,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonManager(bool)));
    connect(ui->m_qpushbutton_field_add,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonManager(bool)));
    connect(ui->m_qpushbutton_field_sub,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonManager(bool)));
    connect(ui->m_qpushbutton_manager_init,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonManager(bool)));
    connect(ui->m_qpushbutton_manager_save,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonManager(bool)));

    connect(ui->m_qcombobox_node,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangedComboboxNode(int)));
    connect(ui->m_qcombobox_field,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangedComboboxNode(int)));
    connect(this->m_qfontdialog_window,SIGNAL(currentFontChanged(QFont)),this,SLOT(slotChangedDialogFont(QFont)));

    connect(ui->m_qpushbutton_node_save,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonNode(bool)));
    connect(ui->m_qpushbutton_node_init,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonNode(bool)));
    connect(ui->m_qradiobutton_un,SIGNAL(clicked(bool)),this,SLOT(slotClickedRadioNode(bool)));
    connect(ui->m_qradiobutton_en,SIGNAL(clicked(bool)),this,SLOT(slotClickedRadioNode(bool)));
    connect(ui->m_qradiobutton_flash,SIGNAL(clicked(bool)),this,SLOT(slotClickedRadioNode(bool)));

    connect(ui->m_qcombobox_alarmlevel,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangedComboboxColor(int)));
    connect(ui->m_qpushbutton_alarm_text,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_alarm_back,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_alarm_flash_text,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_alarm_flash_back,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_recover_text,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_recover_back,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_recover_flash_text,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_recover_flash_back,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonColor(bool)));
    connect(ui->m_qpushbutton_color_save,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonMColor(bool)));
    connect(ui->m_qpushbutton_color_init,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonMColor(bool)));
    connect(ui->m_qpushbutton_color_copy,SIGNAL(clicked(bool)),this,SLOT(slotClickedButtonMColor(bool)));
}

ConfigDialog::~ConfigDialog()
{
    deepClean(*ui->m_qtablewidget_field);
    deepClean(*ui->m_qtablewidget_node);
    delete ui;
}

void ConfigDialog::initData()
{
    this->m_cfdb_service.getDataMap(this->m_ndomain_id,NODE_CONFIG,&this->m_qmap_config);
    this->m_cfdb_service.getDataMap(this->m_ndomain_id,COLOR_CONFIG,&this->m_qmap_color);
    orderData();
    this->m_qmap_config_current.operator =(this->m_qmap_config);
    this->m_qmap_color_current.operator =(this->m_qmap_color);
    return;
}

void ConfigDialog::orderData()
{

}

void ConfigDialog::deepClean(QTableWidget &widget)
{
    int rows = widget.rowCount();
    int columns = widget.columnCount();
    if (0 >= rows || 0 >= columns)
    {
        return;
    }

    rows += 1;
    while (--rows)
    {
        int j = columns + 1;
        while (--j)
        {
            delete widget.item(rows,j);
        }
    }

    widget.setRowCount(0);
    widget.setColumnCount(0);
    widget.clear();
    return;
}

void ConfigDialog::initManagerView()
{
    return initNodeTable(this->m_qmap_config_current);
}

void ConfigDialog::initNodeView()
{
    return initNodeCombobox(this->m_qmap_config_current);

}

void ConfigDialog::initColorView()
{
    setEnableColor(false);
    initAlarmLevelCombobox(this->m_qmap_color_current);
    initColorButton();
    return;
}

void ConfigDialog::initNodeTable(QMap<int, ConfigData> &map)
{
    deepClean(*ui->m_qtablewidget_node);
    setEnableManager(false);

    QMap<int,ConfigData>::iterator iterator_node = map.begin();
    int i = 0;
    ui->m_qtablewidget_node->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->m_qtablewidget_node->setRowCount(map.size());
    ui->m_qtablewidget_node->setColumnCount(2);
    while (map.end() != iterator_node)
    {
        ui->m_qtablewidget_node->setItem(i,0,new QTableWidgetItem(QString::number(iterator_node.value().m_nconfig_id)));
        ui->m_qtablewidget_node->setItem(i,1,new QTableWidgetItem(iterator_node.value().m_qstr_config_name));
        ui->m_qtablewidget_node->item(i,0)->setFlags(Qt::NoItemFlags);
        ++i;
        ++iterator_node;
    }
    QStringList qstrlist_header;
    qstrlist_header.append("节点序号");
    qstrlist_header.append("节点名称");
    ui->m_qtablewidget_node->setHorizontalHeaderLabels(qstrlist_header);
    return;
}

void ConfigDialog::initFieldsTable(QMap<int, QString> &map)
{
    setEnableManager(true);
    ui->m_qtablewidget_field->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->m_qtablewidget_field->clear();
    ui->m_qtablewidget_field->setRowCount(map.size());
    ui->m_qtablewidget_field->setColumnCount(2);

    QMap<int,QString>::iterator iterator_field = map.begin();
    int i = 0;
    while (map.end() != iterator_field)
    {
        ui->m_qtablewidget_field->setItem(i,0,new QTableWidgetItem(QString::number(iterator_field.key())));
        ui->m_qtablewidget_field->setItem(i,1,new QTableWidgetItem(iterator_field.value()));
        ++i;
        ++iterator_field;
    }

    QStringList qstrlist_header;
    qstrlist_header.append("列序号");
    qstrlist_header.append("列名称");
    ui->m_qtablewidget_field->setHorizontalHeaderLabels(qstrlist_header);
    return;
}

void ConfigDialog::initNodeCombobox(const QMap<int, ConfigData> &map)
{
    ui->m_qcombobox_node->clear();
    QMap<int,ConfigData>::const_iterator iterator_node = map.begin();
    int i = 0;
    while (map.end() != iterator_node)
    {
        ui->m_qcombobox_node->insertItem(i,iterator_node.value().m_qstr_config_name);
        ++i;
        ++iterator_node;
    }
    setEnableNode(false);
    ui->m_qcombobox_node->setCurrentIndex(-1);
    ui->m_qcombobox_node->setCursor(this->cursor());
    return;
}

void ConfigDialog::initFieldsCombobox(QMap<int, QString> &map)
{
    ui->m_qcombobox_field->clear();
    if (0 == map.size())
    {
        setPolicyValue(-1);
        ui->m_qgroupbox_viewpolicy->setEnabled(false);
        return;
    }

    QMap<int,QString>::iterator iterator_field = map.begin();
    int i = 0;
    while (map.end() != iterator_field)
    {
        ui->m_qcombobox_field->insertItem(i,iterator_field.value());
        ++i;
        ++iterator_field;
    }
    ui->m_qgroupbox_viewpolicy->setEnabled(true);
    ui->m_qcombobox_field->setCurrentIndex(-1);
    return;
}

void ConfigDialog::initAlarmLevelCombobox(QMap<int, AlarmColor> &map)
{
    ui->m_qcombobox_alarmlevel->clear();
    if (0 == map.size())
    {
        setEnableColor(false);
        return;
    }

    QMap<int,AlarmColor>::iterator iterator_color = map.begin();
    int i = 0;
    while (map.end() != iterator_color)
    {
        ui->m_qcombobox_alarmlevel->insertItem(i,iterator_color.value().m_qstr_alarm_name);
        ++i;
        ++iterator_color;
    }
    setEnableColor(false);
    ui->m_qcombobox_alarmlevel->setCurrentIndex(-1);
    return;
}

void ConfigDialog::initColorButton()
{
    QPalette qplatte_color;
    qplatte_color.setColor(QPalette::Background,QString("#FFFFFF"));
    ui->m_qpushbutton_alarm_text->setPalette(qplatte_color);
    ui->m_qpushbutton_alarm_back->setPalette(qplatte_color);
    ui->m_qpushbutton_alarm_flash_text->setPalette(qplatte_color);
    ui->m_qpushbutton_alarm_flash_back->setPalette(qplatte_color);
    ui->m_qpushbutton_recover_text->setPalette(qplatte_color);
    ui->m_qpushbutton_recover_back->setPalette(qplatte_color);
    ui->m_qpushbutton_recover_flash_text->setPalette(qplatte_color);
    ui->m_qpushbutton_recover_flash_back->setPalette(qplatte_color);
    return;
}

void ConfigDialog::initFontDialog(const QFont &font)
{
    if (this->m_qfontdialog_window)
    {
        this->m_qfontdialog_window->setCurrentFont(font);
    }

    return;
}

void ConfigDialog::setEnableManager(bool status)
{
    deepClean(*ui->m_qtablewidget_field);
    ui->m_qtablewidget_field->setEnabled(status);
    ui->m_qpushbutton_field_add->setEnabled(status);
    ui->m_qpushbutton_field_sub->setEnabled(status);
    return;
}

void ConfigDialog::setEnableNode(bool status)
{
    ui->m_qgroupbox_font->setEnabled(status);
    ui->m_qgroupbox_policy->setEnabled(status);
    ui->m_qpushbutton_node_save->setEnabled(status);
    ui->m_qpushbutton_node_init->setEnabled(status);
    return;
}

void ConfigDialog::setPolicyValue(int value)
{
    switch (value)
    {
    case 0:
        return ui->m_qradiobutton_un->setChecked(true);
    case 1:
        return ui->m_qradiobutton_en->setChecked(true);
    case 2:
        return ui->m_qradiobutton_flash->setChecked(true);
    default:
        ui->m_qradiobutton_un->setChecked(false);
        ui->m_qradiobutton_en->setChecked(false);
        ui->m_qradiobutton_flash->setChecked(false);
        break;
    }
    return;
}

void ConfigDialog::setEnableColor(bool status)
{
    ui->m_qgroupbox_alarm->setEnabled(status);
    ui->m_qgroupbox_recover->setEnabled(status);
    ui->m_qpushbutton_color_save->setEnabled(status);
    ui->m_qpushbutton_color_init->setEnabled(status);
    ui->m_qpushbutton_color_copy->setEnabled(status);
    return;
}

void ConfigDialog::setColorValue(const AlarmColor &color)
{
    QPalette qpalette_color;
    qpalette_color.setColor(QPalette::Button,color.m_qstr_alarm_text);
    ui->m_qpushbutton_alarm_text->setPalette(qpalette_color);
    ui->m_qpushbutton_alarm_text->setFlat(true);

    qpalette_color.setColor(QPalette::Button,color.m_qstr_alarm_back);
    ui->m_qpushbutton_alarm_back->setPalette(qpalette_color);
    ui->m_qpushbutton_alarm_back->setFlat(true);

    qpalette_color.setColor(QPalette::Button,color.m_qstr_alarm_flash_text);
    ui->m_qpushbutton_alarm_flash_text->setPalette(qpalette_color);
    ui->m_qpushbutton_alarm_flash_text->setFlat(true);

    qpalette_color.setColor(QPalette::Button,color.m_qstr_alarm_flash_back);
    ui->m_qpushbutton_alarm_flash_back->setPalette(qpalette_color);
    ui->m_qpushbutton_alarm_flash_back->setFlat(true);

    qpalette_color.setColor(QPalette::Button,color.m_qstr_recover_text);
    ui->m_qpushbutton_recover_text->setPalette(qpalette_color);
    ui->m_qpushbutton_recover_text->setFlat(true);

    qpalette_color.setColor(QPalette::Button,color.m_qstr_recover_back);
    ui->m_qpushbutton_recover_back->setPalette(qpalette_color);
    ui->m_qpushbutton_recover_back->setFlat(true);

    qpalette_color.setColor(QPalette::Button,color.m_qstr_recover_flash_text);
    ui->m_qpushbutton_recover_flash_text->setPalette(qpalette_color);
    ui->m_qpushbutton_recover_flash_text->setFlat(true);

    qpalette_color.setColor(QPalette::Button,color.m_qstr_recover_flash_back);
    ui->m_qpushbutton_recover_flash_back->setPalette(qpalette_color);
    ui->m_qpushbutton_recover_flash_back->setFlat(true);
    return;
}

QVariant ConfigDialog::setColorButton(QPushButton *button)
{
    QVariant qvariant_color;
    if (!button)
    {
        return qvariant_color;
    }

    qvariant_color.setValue(QColorDialog::getColor(button->palette().color(QPalette::Button),this,"",QColorDialog::ShowAlphaChannel));
    QPalette qpalette_color;
    qpalette_color.setColor(QPalette::Button,qvariant_color.toString());
    button->setPalette(qpalette_color);
    button->setFlat(true);
    return qvariant_color;
}

bool ConfigDialog::isExistFieldKey(const int key)
{
    for (int i = 0;ui->m_qtablewidget_field->rowCount() > i;i++)
    {
        if (!ui->m_qtablewidget_field->item(i,0) || !ui->m_qtablewidget_field->item(i,1))
        {
            continue;
        }

        if (ui->m_qtablewidget_field->item(i,0)->text().isEmpty())
        {
            continue;
        }

        if (ui->m_qtablewidget_field->item(i,0)->text().toInt() == key)
        {
            return true;
        }
    }
    return false;
}

bool ConfigDialog::isExistNodeKey(const int key)
{
    for (int i = 0;ui->m_qtablewidget_node->rowCount() > i;i++)
    {
        if (!ui->m_qtablewidget_node->item(i,0) || !ui->m_qtablewidget_node->item(i,1))
        {
            continue;
        }

        if (ui->m_qtablewidget_node->item(i,0)->text().isEmpty())
        {
            continue;
        }

        if (ui->m_qtablewidget_node->item(i,0)->text().toInt() == key)
        {
            return true;
        }
    }
    return false;
}

void ConfigDialog::slotDoubleClickedHeader(int index)
{
    setEnableManager(true);
    ConfigData *configdata_temp = NULL;
    configdata_temp = getCurrentNode(index);
    ui->m_qtablewidget_node->setCurrentItem(ui->m_qtablewidget_node->item(index,1));
    if (configdata_temp)
    {
        initFieldsTable(configdata_temp->m_qmap_fields);
    }
    else
    {
        if (ui->m_qtablewidget_node->item(index,0)->text().isEmpty())
        {
            return;
        }
        else
        {
            if (m_qmap_config_current.contains(ui->m_qtablewidget_node->item(index,0)->text().toInt()))
            {
                return;
            }
            else
            {
                ConfigData data;
                data.m_nconfig_id = ui->m_qtablewidget_node->item(index + 1,0)->text().toInt();
                m_qmap_config_current.insert(data.m_nconfig_id,data);
            }
        }
        slotClickedHeader(index);
    }
    return;
}

void ConfigDialog::slotClickedHeader(int index)
{
    deepClean(*ui->m_qtablewidget_field);
    QStringList qstrlist_header;
    qstrlist_header.append("列序号");
    qstrlist_header.append("列名称");
    ui->m_qtablewidget_field->setHorizontalHeaderLabels(qstrlist_header);
    setEnableManager(false);
    return;
}

void ConfigDialog::slotChangedSelectionItems()
{
    if (0 == ui->m_qtablewidget_node->selectedItems().size())
    {
        setEnableManager(false);
    }
    return;
}

void ConfigDialog::slotClickedItem(QModelIndex index)
{

}

void ConfigDialog::slotClickedItem(int row, int column)
{
    QString qstr_sendername = sender()->objectName();
    if (ui->m_qtablewidget_node->objectName() == qstr_sendername)
    {
        if (!ui->m_qtablewidget_node->item(row,0) || !ui->m_qtablewidget_node->item(row,1))
        {
            return;
        }

        ConfigData *pconfigdata_temp = NULL;
        pconfigdata_temp = getCurrentNode(row);
        if (pconfigdata_temp)
        {
            if (1 == column)
            {
                if (pconfigdata_temp->m_qstr_config_name == ui->m_qtablewidget_node->item(row,column)->text())
                {
                    return;
                }
                pconfigdata_temp->m_qstr_config_name.operator =(ui->m_qtablewidget_node->item(row,column)->text());
            }
            else
            {
                return;
            }
        }
        else
        {
            if (0 == column)
            {
                ConfigData configdata_temp;
                configdata_temp.m_nconfig_id = ui->m_qtablewidget_node->item(row,column)->text().toInt();
                configdata_temp.m_qstr_config_name = ui->m_qtablewidget_node->item(row,1)->text();

                QMap<int,ConfigData>::iterator iterator_node = this->m_qmap_config_current.begin();
                while (this->m_qmap_config_current.end() != iterator_node)
                {
                    if (!isExistNodeKey(iterator_node.value().m_nconfig_id))
                    {
                        configdata_temp.m_qmap_fields.operator =(iterator_node.value().m_qmap_fields);
                        configdata_temp.m_qmap_policy.operator =(iterator_node.value().m_qmap_policy);
                        this->m_qmap_config_current.erase(iterator_node);
                        break;
                    }
                    ++iterator_node;
                }
                this->m_qmap_config_current.insert(this->m_qmap_config_current.size(),configdata_temp);
            }
        }
    }
    else if (ui->m_qtablewidget_field->objectName() == qstr_sendername)
    {
        if (!ui->m_qtablewidget_field->item(row,0) || !ui->m_qtablewidget_field->item(row,1))
        {
            return;
        }

        int nnode_row = ui->m_qtablewidget_node->selectedItems().first()->row();
        ConfigData *pconfigdata_temp = NULL;
        pconfigdata_temp = getCurrentNode(nnode_row);
        if (!pconfigdata_temp)
        {
            return;
        }
        if (ui->m_qtablewidget_field->item(row,0)->text().isEmpty())
        {
            return;
        }
        if (0 == column)
        {
            QMap<int,QString>::iterator iterator_field = pconfigdata_temp->m_qmap_fields.begin();
            while (pconfigdata_temp->m_qmap_fields.end() != iterator_field)
            {
                if (!isExistFieldKey(iterator_field.key()))
                {
                    if (pconfigdata_temp->m_qmap_fields.contains(iterator_field.key()))
                    {
                        pconfigdata_temp->m_qmap_fields.remove(iterator_field.key());
                        break;
                    }
                }
                ++iterator_field;
            }
        }

        if (pconfigdata_temp->m_qmap_fields.contains(ui->m_qtablewidget_field->item(row,0)->text().toInt()))
        {
            if (1 == column)
            {
                pconfigdata_temp->m_qmap_fields[ui->m_qtablewidget_field->item(row,0)->text().toInt()] = ui->m_qtablewidget_field->item(row,1)->text();
            }
        }
        else
        {
            if (0 == column)
            {
                int nkey = ui->m_qtablewidget_field->item(row,0)->text().toInt();
                QString qstr_name = ui->m_qtablewidget_field->item(row,1)->text();
                pconfigdata_temp->m_qmap_fields.insert(nkey,qstr_name);
                pconfigdata_temp->m_qmap_policy.insert(nkey,2);
            }
        }
    }
    return;
}

void ConfigDialog::slotClickedButtonManager(bool status)
{
    QString qstr_sendername = sender()->objectName();
    if (ui->m_qpushbutton_node_add->objectName() == qstr_sendername)
    {
        ui->m_qtablewidget_node->setRowCount(ui->m_qtablewidget_node->rowCount()+1);
        ui->m_qtablewidget_node->setColumnCount(2);
        ui->m_qtablewidget_node->setItem(ui->m_qtablewidget_node->rowCount()-1,0,new QTableWidgetItem(QString("")));
        ui->m_qtablewidget_node->setItem(ui->m_qtablewidget_node->rowCount()-1,1,new QTableWidgetItem(QString("")));
        int id = 1;
        while(1)
        {
            if (m_qmap_config_current.contains(id))
            {
                ++id;
            }
            else
            {
                break;
            }
        }
        ui->m_qtablewidget_node->item(ui->m_qtablewidget_node->rowCount()-1,0)->setText(QString::number(id));
        ui->m_qtablewidget_node->item(ui->m_qtablewidget_node->rowCount()-1,0)->setFlags(Qt::NoItemFlags);
    }
    else if (ui->m_qpushbutton_node_sub->objectName() == qstr_sendername)
    {
        int nitems_count = 0;
        nitems_count = ui->m_qtablewidget_node->selectedItems().size();
        if (0 == nitems_count)
        {
            return;
        }
        nitems_count += 1;
        int nstart = 0;
        while (--nitems_count)
        {
            QMap<int,ConfigData>::iterator iterator_config = this->m_qmap_config_current.begin();
            while (this->m_qmap_config_current.end() != iterator_config)
            {
                if (ui->m_qtablewidget_node->item(ui->m_qtablewidget_node->selectedItems().at(nstart)->row(),0)->text().toInt() ==iterator_config.value().m_nconfig_id)
                {
                    this->m_qmap_config_current.remove(iterator_config.key());
                    break;
                }
                ++iterator_config;
            }
            ui->m_qtablewidget_node->removeRow(ui->m_qtablewidget_node->selectedItems().at(nstart)->row());
            nstart += 2;
        }
    }
    else if (ui->m_qpushbutton_field_add->objectName() == qstr_sendername)
    {
        if (!ui->m_qtablewidget_field->isEnabled())
        {
            return;
        }

        int nnode_row = ui->m_qtablewidget_node->selectedItems().first()->row();
        ConfigData *configdata_temp = NULL;
        configdata_temp = getCurrentNode(nnode_row);
        if (!configdata_temp)
        {
            return;
        }

        ui->m_qtablewidget_field->setRowCount(ui->m_qtablewidget_field->rowCount()+1);
        ui->m_qtablewidget_field->setColumnCount(2);
        ui->m_qtablewidget_field->setItem(ui->m_qtablewidget_field->rowCount()-1,0,new QTableWidgetItem(""));
        ui->m_qtablewidget_field->setItem(ui->m_qtablewidget_field->rowCount()-1,1,new QTableWidgetItem(""));

    }
    else if (ui->m_qpushbutton_field_sub->objectName() == qstr_sendername)
    {
        int nnode_row = ui->m_qtablewidget_node->selectedItems().first()->row();
        ConfigData *configdata_temp = NULL;
        configdata_temp = getCurrentNode(nnode_row);
        if (!configdata_temp)
        {
            return;
        }

        int nfield_count = ui->m_qtablewidget_field->selectedItems().size();
        if (0 == nfield_count)
        {
            return;
        }

        qDebug() << "slotClickedButtonManager";
        if (configdata_temp->m_qmap_fields.contains(ui->m_qtablewidget_field->selectedItems().at(0)->text().toInt()))
        {
            configdata_temp->m_qmap_fields.remove(ui->m_qtablewidget_field->selectedItems().at(0)->text().toInt());
        }

        ui->m_qtablewidget_field->removeRow(ui->m_qtablewidget_field->selectedItems().at(0)->row());
    }
    else if (ui->m_qpushbutton_manager_init->objectName() == qstr_sendername)
    {
        this->m_qmap_config_current.operator =(this->m_qmap_config);
        initManagerView();
    }
    else if (ui->m_qpushbutton_manager_save->objectName() == qstr_sendername)
    {
        this->m_cfdb_service.setDataMap(this->m_ndomain_id,NODE_MANAGER,&this->m_qmap_config_current);
        this->m_qmap_config.operator =(this->m_qmap_config_current);
    }
    return;
}

ConfigData *ConfigDialog::getCurrentNode()
{
    QString qstr_nodename = ui->m_qcombobox_node->currentText();
    if (qstr_nodename.isEmpty())
    {
        return NULL;
    }

    QMap<int,ConfigData>::iterator iterator_node = this->m_qmap_config_current.begin();
    while (this->m_qmap_config_current.end() != iterator_node)
    {
        if (qstr_nodename == iterator_node.value().m_qstr_config_name)
        {
            return &this->m_qmap_config_current[iterator_node.key()];
        }
        ++iterator_node;
    }
    return NULL;
}

AlarmColor *ConfigDialog::getCurrentColor()
{
    QString qstr_levelname = ui->m_qcombobox_alarmlevel->currentText();
    if (qstr_levelname.isEmpty())
    {
        return NULL;
    }

    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_color_current.begin();
    while (this->m_qmap_color_current.end() != iterator_color)
    {
        if (qstr_levelname == iterator_color.value().m_qstr_alarm_name)
        {
            return &this->m_qmap_color_current[iterator_color.key()];
        }
        ++iterator_color;
    }
    return NULL;
}

ConfigData *ConfigDialog::getCurrentNode(int row)
{
    if (0 > row || ui->m_qtablewidget_node->item(row,0)->text().isEmpty())
    {
        return NULL;
    }

    QMap<int,ConfigData>::iterator iterator_config = this->m_qmap_config_current.begin();
    while (this->m_qmap_config_current.end() != iterator_config)
    {
        if (ui->m_qtablewidget_node->item(row,0)->text().toInt() == iterator_config.value().m_nconfig_id)
        {
            return &this->m_qmap_config_current[iterator_config.key()];
        }

        ++iterator_config;
    }
    return NULL;
}

void ConfigDialog::slotClickedButtonNode(bool status)
{
    ConfigData *pconfigdata_temp = NULL;
    pconfigdata_temp = getCurrentNode();
    if (!pconfigdata_temp)
    {
        return;
    }

    QString qstr_sendname = this->sender()->objectName();
    if (qstr_sendname == ui->m_qpushbutton_node_save->objectName())
    {
        if (0 == this->m_cfdb_service.setData(this->m_ndomain_id,NODE_CONFIG,getCurrentNode()))
        {
            QMap<int,ConfigData>::iterator iterator_config = this->m_qmap_config.begin();
            while (this->m_qmap_config.end() != iterator_config)
            {
                if (getCurrentNode()->m_nconfig_id == iterator_config.value().m_nconfig_id)
                {
                    this->m_qmap_config[iterator_config.key()].operator =(*getCurrentNode());
                    break;
                }
                ++iterator_config;
            }
        }
    }
    else if (qstr_sendname == ui->m_qpushbutton_node_init->objectName())
    {
        QMap<int,ConfigData>::iterator iterator_config = this->m_qmap_config.begin();
        while(this->m_qmap_config.end() != iterator_config)
        {
            if(getCurrentNode()->m_nconfig_id == iterator_config.value().m_nconfig_id)
            {
                getCurrentNode()->operator =(this->m_qmap_config[iterator_config.key()]);
                emit ui->m_qcombobox_node->currentIndexChanged(ui->m_qcombobox_node->currentIndex());
                return;
            }
            ++iterator_config;
        }
    }
    return;
}

void ConfigDialog::slotClickedRadioNode(bool status)
{
    QString qstr_sendname = this->sender()->objectName();
    QString qstr_fieldname = ui->m_qcombobox_field->currentText();
    if (qstr_fieldname.isEmpty())
    {
        return;
    }

    int nvalue = 1;
    if (qstr_sendname == ui->m_qradiobutton_un->objectName())
    {
        nvalue = 0;
    }
    else if (qstr_sendname == ui->m_qradiobutton_en->objectName())
    {
        nvalue = 1;
    }
    else if (qstr_sendname == ui->m_qradiobutton_flash->objectName())
    {
        nvalue = 2;
    }

    ConfigData *pconfigdata_temp = NULL;
    pconfigdata_temp = getCurrentNode();
    if (!pconfigdata_temp)
    {
        return;
    }

    QMap<int,QString>::iterator iterator_fields = pconfigdata_temp->m_qmap_fields.begin();
    while (pconfigdata_temp->m_qmap_fields.end() != iterator_fields)
    {
        if (qstr_fieldname == iterator_fields.value())
        {
            if (!pconfigdata_temp->m_qmap_policy.contains(iterator_fields.key()))
            {
                return;
            }

            pconfigdata_temp->m_qmap_policy[iterator_fields.key()] = nvalue;
            return;
        }
        ++iterator_fields;
    }
    return;
}

void ConfigDialog::slotChangedComboboxNode(int index)
{
    Q_UNUSED(index);
    QString qstr_sendname = this->sender()->objectName();
    if (qstr_sendname == ui->m_qcombobox_node->objectName())
    {
        ConfigData *pconfigdata_temp = NULL;
        pconfigdata_temp = getCurrentNode();
        if (!pconfigdata_temp)
        {
            return setEnableNode(false);
        }

        initFieldsCombobox(pconfigdata_temp->m_qmap_fields);
        QFont qfont_text;
        qfont_text.fromString(pconfigdata_temp->m_qstr_text_font);
        initFontDialog(qfont_text);
        return setEnableNode(true);
    }
    else if (qstr_sendname == ui->m_qcombobox_field->objectName())
    {
        QString qstr_fieldname = ui->m_qcombobox_field->currentText();
        if(qstr_fieldname.isEmpty())
        {
            ui->m_qgroupbox_viewpolicy->setEnabled(false);
            return;
        }

        ConfigData *pconfigdata_temp = NULL;
        pconfigdata_temp = getCurrentNode();
        if (!pconfigdata_temp)
        {
            ui->m_qgroupbox_viewpolicy->setEnabled(false);
            return;
        }

        QMap<int,QString>::iterator iterator_fields = pconfigdata_temp->m_qmap_fields.begin();
        while (pconfigdata_temp->m_qmap_fields.end() != iterator_fields)
        {
            if (qstr_fieldname == iterator_fields.value())
            {
                setPolicyValue(pconfigdata_temp->m_qmap_policy.value(iterator_fields.key()));
                ui->m_qgroupbox_viewpolicy->setEnabled(true);
                return;
            }
            ++iterator_fields;
        }
    }
    return;
}

void ConfigDialog::slotChangedDialogFont(QFont font)
{
    getCurrentNode()->m_qstr_text_font.operator =(font.toString());
    return;
}

void ConfigDialog::slotClickedButtonColor(bool status)
{
    AlarmColor *palarmcolor_temp = NULL;
    palarmcolor_temp = getCurrentColor();
    if (!palarmcolor_temp)
    {
        return;
    }

    QString qstr_sendname = this->sender()->objectName();

    if (qstr_sendname == ui->m_qpushbutton_alarm_text->objectName())
    {
        palarmcolor_temp->m_qstr_alarm_text = setColorButton(ui->m_qpushbutton_alarm_text).toString();
    }
    else if (qstr_sendname == ui->m_qpushbutton_alarm_back->objectName())
    {
        palarmcolor_temp->m_qstr_alarm_back = setColorButton(ui->m_qpushbutton_alarm_back).toString();
    }
    else if (qstr_sendname == ui->m_qpushbutton_alarm_flash_text->objectName())
    {
        palarmcolor_temp->m_qstr_alarm_flash_text = setColorButton(ui->m_qpushbutton_alarm_flash_text).toString();
    }
    else if (qstr_sendname == ui->m_qpushbutton_alarm_flash_back->objectName())
    {
        palarmcolor_temp->m_qstr_alarm_flash_back = setColorButton(ui->m_qpushbutton_alarm_flash_back).toString();
    }
    else if (qstr_sendname == ui->m_qpushbutton_recover_text->objectName())
    {
        palarmcolor_temp->m_qstr_recover_text = setColorButton(ui->m_qpushbutton_recover_text).toString();
    }
    else if (qstr_sendname == ui->m_qpushbutton_recover_back->objectName())
    {
        palarmcolor_temp->m_qstr_recover_back = setColorButton(ui->m_qpushbutton_recover_back).toString();
    }
    else if (qstr_sendname == ui->m_qpushbutton_recover_flash_text->objectName())
    {
        palarmcolor_temp->m_qstr_recover_flash_text = setColorButton(ui->m_qpushbutton_recover_flash_text).toString();
    }
    else if (qstr_sendname == ui->m_qpushbutton_recover_flash_back->objectName())
    {
        palarmcolor_temp->m_qstr_recover_flash_back = setColorButton(ui->m_qpushbutton_recover_flash_back).toString();
    }
    return;
}

void ConfigDialog::slotClickedButtonMColor(bool status)
{
    QString qstr_sendname = this->sender()->objectName();
    if (qstr_sendname == ui->m_qpushbutton_color_save->objectName())
    {
        if (0 == this->m_cfdb_service.setData(this->m_ndomain_id,COLOR_CONFIG,getCurrentColor()))
        {
            this->m_qmap_color[getCurrentColor()->m_nalarm_level].operator =(*getCurrentColor());
        }
    }
    else if (qstr_sendname == ui->m_qpushbutton_color_init->objectName())
    {
        if(getCurrentColor())
        {
            getCurrentColor()->operator =(this->m_qmap_color.value(getCurrentColor()->m_nalarm_level));
            setColorValue(*getCurrentColor());
        }
    }
    else if (qstr_sendname == ui->m_qpushbutton_color_copy->objectName())
    {
        ColorCopyDialog colorcopydialog_window(this->m_qmap_color_current);
        colorcopydialog_window.exec();
        emit ui->m_qcombobox_alarmlevel->currentIndexChanged(ui->m_qcombobox_alarmlevel->currentIndex());
    }
    return;
}

void ConfigDialog::slotChangedComboboxColor(int index)
{
    Q_UNUSED(index);
    AlarmColor *palarmcolor_temp = NULL;
    palarmcolor_temp = getCurrentColor();
    if (!palarmcolor_temp)
    {
        qDebug() << "current color is not.";
        return;
    }
    setEnableColor(true);
    setColorValue(*palarmcolor_temp);
    return;
}
