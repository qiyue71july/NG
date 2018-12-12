#ifndef COMMNSTRUCT_H
#define COMMNSTRUCT_H

#include <QList>
#include <QMap>
#include <QObject>

struct stru_group_specfunc_area
{
    QList<int> m_vector_perm_group_station;
    QList<int> m_vector_perm_group_region;
};

//用户信息 待定
typedef struct user_info
{
    int m_nuserid = 1;
    int m_nusergid = 1;
    int m_ndomainid = 1;
    int m_nstationid = 1;
    QMap<int,QString> m_map_permission;
    QMap<QString,stru_group_specfunc_area> m_map_specfunc;
    QString m_qstr_hostname;
    bool is_occ;
}UserInfo;

enum ConfigIdStruct
{
    CONFIGID_ALARMMAIN = 1,CONFIGID_ALARMSUMMARY = 2,CONFIGID_EVENTMAIN
};

enum ConfigFields
{
    CONFIGFIELDS_ALARMMAIN = 13,CONFIGFIELDS_ALARMSUMMARY = 13,CONFIGFIELDS_EVENTMAIN = 12
};

enum PermStruct
{
    PERM_STATION = 0,PERM_REGION
};

//报警颜色
class AlarmColour
{
public:
    void Clear();
    AlarmColour& operator =(const AlarmColour& dst);

    int m_nlevel;
    QString m_qstr_levelname;
    QString m_qstr_alarm_text;
    QString m_qstr_alarm_background;
    QString m_qstr_alarm_flash_text;
    QString m_qstr_alarm_flash_background;
    QString m_qstr_recover_text;
    QString m_qstr_recover_background;
    QString m_qstr_recover_flash_text;
    QString m_qstr_recover_flash_background;
};

//报警配置
class AlarmConfig
{
public:
    void Clear();
    AlarmConfig& operator =(const AlarmConfig& dst);

    int m_nnodekey = -1;
    QString m_qstr_nodename;
    QMap<int,QString> m_map_field;
    QMap<int,int> m_map_policy;
    QMap<int,AlarmColour> m_map_color;
    QString m_qstr_font;
    QString m_qstr_info;
};

#endif // COMMNSTRUCT_H
