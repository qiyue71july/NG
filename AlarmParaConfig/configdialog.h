#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QTreeView>
#include <QPushButton>
#include <QFrame>
#include <QWidget>
#include <QThread>
#include <QVariant>
#include <QFontDialog>
#include <QTableWidget>

#include "configstruct.h"
#include "cfdbservice.h"
#include "colorcopydialog.h"
#include "logindialog.h"


namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

private:
    //初始化数据
    void initData();
    void orderData();
    void deepClean(QTableWidget &widget);

    //******初始化界面******
    //初始化管理界面
    void initManagerView();
    //初始化配置界面
    void initNodeView();
    void initColorView();
    //************************


    //******初始化控件******
    //初始化管理界面控件
    void initNodeTable(QMap<int,ConfigData> &map);
    void initFieldsTable(QMap<int,QString> &map);
    //初始化配置界面控件
    void initNodeCombobox(const QMap<int,ConfigData> &map);
    void initFieldsCombobox(QMap<int,QString> &map);
    void initAlarmLevelCombobox(QMap<int, AlarmColor> &map);
    void initColorButton();
    void initFontDialog(const QFont &font);
    //************************

    //******设置参数******
    //设置管理界面参数
    void setEnableManager(bool status);
    //设置配置界面参数
    void setEnableNode(bool status);
    void setPolicyValue(int value);
    void setEnableColor(bool status);
    void setColorValue(const AlarmColor &color);
    //************************

    //******获取结构体******
    //获取配置结构体
    ConfigData *getCurrentNode();
    AlarmColor *getCurrentColor();
    //获取管理结构体
    ConfigData *getCurrentNode(int row);
    //************************

    //设置按钮
    QVariant setColorButton(QPushButton *button);
    //判断manager
    bool isExistFieldKey(const int key);
    bool isExistNodeKey(const int key);

private slots:
    //manager
    void slotDoubleClickedHeader(int index);
    void slotClickedHeader(int index);
    void slotChangedSelectionItems();
    void slotClickedItem(QModelIndex index);
    void slotClickedItem(int row,int column);
    void slotClickedButtonManager(bool status);
    //node config
    void slotClickedButtonNode(bool status);
    void slotClickedRadioNode(bool status);
    void slotChangedComboboxNode(int index);
    void slotChangedDialogFont(QFont font);
    //color config
    void slotClickedButtonColor(bool status);
    void slotClickedButtonMColor(bool status);
    void slotChangedComboboxColor(int index);

private:
    Ui::ConfigDialog *ui;
    CfdbService m_cfdb_service;
    int m_ndomain_id = 1;
    int m_nlogin_tag = -1;
    QMap<int,ConfigData> m_qmap_config;
    QMap<int,ConfigData> m_qmap_config_current;
    QMap<int,AlarmColor> m_qmap_color;
    QMap<int,AlarmColor> m_qmap_color_current;
    QFontDialog *m_qfontdialog_window = NULL;
};

#endif // CONFIGDIALOG_H
