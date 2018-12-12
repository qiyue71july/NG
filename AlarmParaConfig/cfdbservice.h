#ifndef CFDBSERVICE_H
#define CFDBSERVICE_H

#include <QObject>
#include "confcmn.h"
#include "configstruct.h"
#include "commnstruct.h"

class CfdbService : public QObject
{
    Q_OBJECT
public:
    explicit CfdbService(QObject *parent = nullptr);
    ~CfdbService();
    int setDataMap(int domianid, int type, void *data);
    int getDataMap(int domianid,int type,void *data);
    int setData(int domianid,int type,void *data);

private:
    int setConfigData(int domianid, ConfigData &data);
    int updateConfigData(int domianid,ConfigData &data);
    int updateColorData(int domianid,AlarmColor &data);
    int getConfigData(int domianid,ConfigData &data);
    int getColorData(int domianid,AlarmColor &data);

    int setConfigData(int domianid,QMap<int,ConfigData> &data);
    int updateConfigData(int domianid,QMap<int,ConfigData> &data);
    int updateColorData(int domianid,QMap<int,AlarmColor> &data);
    int getConfigData(int domianid,QMap<int,ConfigData> &data);
    int getColorData(int domianid,QMap<int,AlarmColor> &data);

    bool isValidDomain(int domainid) const;

private:
    QString m_qstrquery_alarmconfigtbl = QString("select * from conf.alarm_config_tbl");
    QString m_qstrquery_alarmleveltbl = QString("select * from conf.alarm_level_tbl");
    QString m_qstrinsert_alarmconfigtbl = QString("insert into conf.alarm_config_tbl (config_id,config_name,text_font,config_info) values('%1','%2','%3','%4')");
    QString m_qstrupdate_alarmconfigtbl = QString("UPDATE conf.alarm_config_tbl SET config_name='%1',text_font='%2',config_info='%3' WHERE config_id='%4'");
    QString m_qstrupdate_alarmleveltbl = QString("UPDATE conf.alarm_level_tbl SET alarm_text_color = '%1' , alarm_background_color = '%2' , alarm_flash_text_color = '%3' , alarm_flash_background_color = '%4' ,"
                                               "recover_text_color = '%5' , recover_background_color = '%6' , recover_flash_text_color = '%7' , recover_flash_background_color = '%8' WHERE level_id = '%9'");
    QString m_qstrdelete_alarmconfigtbl = QString("TRUNCATE conf.alarm_config_tbl");
};

#endif // CFDBSERVICE_H
