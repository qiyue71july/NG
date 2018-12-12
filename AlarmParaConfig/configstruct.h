#ifndef CONFIGSTRUCT_H
#define CONFIGSTRUCT_H
#include <QString>
#include <QMap>

typedef struct config_data
{
    void Clear();
    config_data &operator =(const config_data& dst);
    int m_nconfig_id = 0;
    QString m_qstr_config_name;
    QString m_qstr_text_font;
    QString m_qstr_config_info;
    QMap<int,QString> m_qmap_fields;
    QMap<int,int> m_qmap_policy;
}ConfigData;

typedef struct alarm_color
{
    alarm_color &operator =(const alarm_color& dst);
    int m_nalarm_level = 0;
    QString m_qstr_alarm_name;
    QString m_qstr_alarm_text = QString("#FF0000");
    QString m_qstr_alarm_back = QString("#000000");
    QString m_qstr_alarm_flash_text = QString("#FF0000");
    QString m_qstr_alarm_flash_back = QString("#000000");
    QString m_qstr_recover_text = QString("#FF0000");
    QString m_qstr_recover_back = QString("#000000");
    QString m_qstr_recover_flash_text = QString("#FF0000");
    QString m_qstr_recover_flash_back = QString("#000000");
}AlarmColor;

enum ConfigType
{
    NODE_MANAGER = 0,NODE_CONFIG,COLOR_CONFIG
};

#endif // CONFIGSTRUCT_H
