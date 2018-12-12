#include "cfdbservice.h"
#include "translateservice.h"
#include <QDebug>

CfdbService::CfdbService(QObject *parent) : QObject(parent)
{
    confdb_init();
}

CfdbService::~CfdbService()
{
    confdb_release();
}

int CfdbService::setDataMap(int domianid, int type, void *data)
{
    if (!isValidDomain(domianid))
    {
        return -1;
    }

    QMap<int,ConfigData> *pconfigdata_temp = NULL;
    QMap<int,AlarmColor> *palarmcolor_temp = NULL;

    switch (type)
    {
    case NODE_MANAGER:
        pconfigdata_temp = (QMap<int,ConfigData> *)data;
        if (pconfigdata_temp)
        {
            return setConfigData(domianid,*pconfigdata_temp);
        }
        break;
    case NODE_CONFIG:
        pconfigdata_temp = (QMap<int,ConfigData> *)data;
        if (pconfigdata_temp)
        {
            return updateConfigData(domianid,*pconfigdata_temp);
        }
        break;
    case COLOR_CONFIG:
        palarmcolor_temp = (QMap<int,AlarmColor> *)data;
        if (palarmcolor_temp)
        {
            return updateColorData(domianid,*palarmcolor_temp);
        }
        break;
    default:
        break;
    }
    return -1;
}

int CfdbService::getDataMap(int domianid, int type, void *data)
{
    if (!isValidDomain(domianid))
    {
        return -1;
    }

    QMap<int,ConfigData> *pconfigdata_temp = NULL;
    QMap<int,AlarmColor> *palarmcolor_temp = NULL;

    switch (type)
    {
    case NODE_MANAGER:
    case NODE_CONFIG:
        pconfigdata_temp = (QMap<int,ConfigData> *)data;
        if (pconfigdata_temp)
        {
            return getConfigData(domianid,*pconfigdata_temp);
        }
        break;
    case COLOR_CONFIG:
        palarmcolor_temp = (QMap<int,AlarmColor> *)data;
        if (palarmcolor_temp)
        {
            return getColorData(domianid,*palarmcolor_temp);
        }
        break;
    default:
        break;
    }
    return -1;
}

int CfdbService::setData(int domianid, int type, void *data)
{
    if (!isValidDomain(domianid))
    {
        return -1;
    }

    ConfigData *pconfigdata_temp = NULL;
    AlarmColor *palarmcolor_temp = NULL;

    switch (type)
    {
    case NODE_MANAGER:
        pconfigdata_temp = (ConfigData *)data;
        if (pconfigdata_temp)
        {
            return setConfigData(domianid,*pconfigdata_temp);
        }
        break;
    case NODE_CONFIG:
        pconfigdata_temp = (ConfigData *)data;
        if (pconfigdata_temp)
        {
            return updateConfigData(domianid,*pconfigdata_temp);
        }
        break;
    case COLOR_CONFIG:
        palarmcolor_temp = (AlarmColor *)data;
        if (palarmcolor_temp)
        {
            return updateColorData(domianid,*palarmcolor_temp);
        }
        break;
    default:
        break;
    }
    return -1;
}

int CfdbService::setConfigData(int domianid, QMap<int, ConfigData> &data)
{
    Q_UNUSED(domianid);
    confdb_exec(m_qstrdelete_alarmconfigtbl.toStdString().c_str());
    QMap<int, ConfigData>::iterator iterator_config = data.begin();
    while (data.end() != iterator_config)
    {
        QString qstr_config_id = QString::number(iterator_config.value().m_nconfig_id);
        QString qstr_config_name = iterator_config.value().m_qstr_config_name;
        QString qstr_text_font = iterator_config.value().m_qstr_text_font;
        TransLateService translateservice;
        QString qstr_config_info = translateservice.transMapToJson(data[iterator_config.key()].m_qmap_fields,data[iterator_config.key()].m_qmap_policy);

        if (qstr_config_name.isEmpty())
        {
            qstr_config_name.append("null");
        }
        if (qstr_text_font.isEmpty())
        {
            qstr_text_font.append("null");
        }
        if (qstr_config_info.isEmpty())
        {
            qstr_config_info.append("null");
        }

        QString qstr_condition;
        qstr_condition.append(this->m_qstrinsert_alarmconfigtbl.arg(qstr_config_id).arg(qstr_config_name).arg(qstr_text_font).arg(qstr_config_info));
        confdb_exec(qstr_condition.toStdString().c_str());
        ++iterator_config;
    }
}

int CfdbService::updateColorData(int domianid, QMap<int, AlarmColor> &data)
{
    Q_UNUSED(domianid);
    QMap<int, AlarmColor>::iterator iterator_color = data.begin();
    while (data.end() != iterator_color)
    {
        QString qstr_condition;
        qstr_condition.append(this->m_qstrupdate_alarmleveltbl.arg(iterator_color.value().m_qstr_alarm_text).arg(iterator_color.value().m_qstr_alarm_back)
                              .arg(iterator_color.value().m_qstr_alarm_flash_text).arg(iterator_color.value().m_qstr_alarm_flash_back)
                              .arg(iterator_color.value().m_qstr_recover_text).arg(iterator_color.value().m_qstr_recover_back)
                              .arg(iterator_color.value().m_qstr_recover_flash_text).arg(iterator_color.value().m_qstr_recover_flash_back)
                              .arg(iterator_color.value().m_nalarm_level));
        confdb_exec(qstr_condition.toStdString().c_str());
        ++iterator_color;
    }
}

int CfdbService::getConfigData(int domianid, QMap<int, ConfigData> &data)
{
    Q_UNUSED(domianid);
    ConfDbRecordSet *cfdb_records = NULL;
    cfdb_records = confdb_query(this->m_qstrquery_alarmconfigtbl.toStdString().c_str());
    char *arr[] = {"config_id", "config_name", "text_font", "config_info", NULL};

    if (cfdb_records)
    {
        int nrows = confdb_recordset_get_count(cfdb_records);
        for (int i = 0;nrows > i;i++)
        {
            ConfigData configdata;
            for (int j = 0;arr[j];j++)
            {
                const char *ch_value = confdb_recordset_get(cfdb_records,i,arr[j]);
                if (!ch_value)
                {
                    continue;
                }

                if (QString("config_id") == QString(arr[j]))
                {
                    configdata.m_nconfig_id = QString(ch_value).toInt();
                }
                else if (QString("config_name") == QString(arr[j]))
                {
                    configdata.m_qstr_config_name = QString(ch_value);
                }
                else if (QString("text_font") == QString(arr[j]))
                {
                    configdata.m_qstr_text_font = QString(ch_value);
                }
                else if (QString("config_info") == QString(arr[j]))
                {
                    configdata.m_qstr_config_info = QString(ch_value);
                    TransLateService translateservice;
                    translateservice.transJsonToMap(configdata.m_qmap_fields,configdata.m_qmap_policy,configdata.m_qstr_config_info);
                }
                ch_value = NULL;
            }
            data.insert(configdata.m_nconfig_id,configdata);
        }
    }

    confdb_recordset_free(cfdb_records);
    return 0;
}

int CfdbService::getColorData(int domianid, QMap<int, AlarmColor> &data)
{
    Q_UNUSED(domianid);
    ConfDbRecordSet *cfdb_records = NULL;
    cfdb_records = confdb_query(m_qstrquery_alarmleveltbl.toStdString().c_str());
    char *arr[] = {"level_id", "level_name", "alarm_text_color", "alarm_background_color",
                   "alarm_flash_text_color", "alarm_flash_background_color", "recover_text_color", "recover_background_color",
                   "recover_flash_text_color", "recover_flash_background_color", NULL};

    if (cfdb_records)
    {
        int nrows = confdb_recordset_get_count(cfdb_records);
        for (int i = 0;nrows > i;i++)
        {
            AlarmColor colordata;
            for (int j = 0;arr[j];j++)
            {
                const char *ch_value = confdb_recordset_get(cfdb_records,i,arr[j]);
                if (!ch_value)
                {
                    continue;
                }

                if (QString("level_id") == QString(arr[j]))
                {
                    colordata.m_nalarm_level = QString(ch_value).toInt();
                }
                else if (QString("level_name") == QString(arr[j]))
                {
                    colordata.m_qstr_alarm_name = QString(ch_value);
                }
                else if (QString("alarm_text_color") == QString(arr[j]))
                {
                    colordata.m_qstr_alarm_text = QString(ch_value);
                }
                else if (QString("alarm_background_color") == QString(arr[j]))
                {
                    colordata.m_qstr_alarm_back = QString(ch_value);
                }
                else if (QString("alarm_flash_text_color") == QString(arr[j]))
                {
                    colordata.m_qstr_alarm_flash_text = QString(ch_value);
                }
                else if (QString("alarm_flash_background_color") == QString(arr[j]))
                {
                    colordata.m_qstr_alarm_flash_back = QString(ch_value);
                }
                else if (QString("recover_text_color") == QString(arr[j]))
                {
                    colordata.m_qstr_recover_text = QString(ch_value);
                }
                else if (QString("recover_background_color") == QString(arr[j]))
                {
                    colordata.m_qstr_recover_back = QString(ch_value);
                }
                else if (QString("recover_flash_text_color") == QString(arr[j]))
                {
                    colordata.m_qstr_recover_flash_text = QString(ch_value);
                }
                else if (QString("recover_flash_background_color") == QString(arr[j]))
                {
                    colordata.m_qstr_recover_flash_back = QString(ch_value);
                }
                ch_value = NULL;
            }
            data.insert(colordata.m_nalarm_level,colordata);
        }
    }

    confdb_recordset_free(cfdb_records);
    return 0;
}

int CfdbService::setConfigData(int domianid, ConfigData &data)
{
    Q_UNUSED(domianid);
    confdb_exec(m_qstrdelete_alarmconfigtbl.toStdString().c_str());
    QString qstr_config_id = QString::number(data.m_nconfig_id);
    QString qstr_config_name = data.m_qstr_config_name;
    QString qstr_text_font = data.m_qstr_text_font;
    TransLateService translateservice;
    QString qstr_config_info = translateservice.transMapToJson(data.m_qmap_fields,data.m_qmap_policy);

    if (qstr_config_name.isEmpty())
    {
        qstr_config_name.append("null");
    }
    if (qstr_text_font.isEmpty())
    {
        qstr_text_font.append("null");
    }
    if (qstr_config_info.isEmpty())
    {
        qstr_config_info.append("null");
    }

    QString qstr_condition;
    qstr_condition.append(this->m_qstrinsert_alarmconfigtbl.arg(qstr_config_id).arg(qstr_config_name).arg(qstr_text_font).arg(qstr_config_info));
    confdb_exec(qstr_condition.toStdString().c_str());
}

int CfdbService::updateConfigData(int domianid, ConfigData &data)
{
    Q_UNUSED(domianid);
    QString qstr_condition;
    TransLateService transservice;
    data.m_qstr_config_info = transservice.transMapToJson(data.m_qmap_fields,data.m_qmap_policy);
    qstr_condition.append(this->m_qstrupdate_alarmconfigtbl.arg(data.m_qstr_config_name).arg(data.m_qstr_text_font).arg(data.m_qstr_config_info).arg(data.m_nconfig_id));
    qDebug() << "updateConfigData" << qstr_condition;
    return confdb_exec(qstr_condition.toStdString().c_str());;
}

int CfdbService::updateColorData(int domianid, AlarmColor &data)
{
    Q_UNUSED(domianid);
    QString qstr_condition;
    qstr_condition.append(this->m_qstrupdate_alarmleveltbl.arg(data.m_qstr_alarm_text).arg(data.m_qstr_alarm_back)
                          .arg(data.m_qstr_alarm_flash_text).arg(data.m_qstr_alarm_flash_back)
                          .arg(data.m_qstr_recover_text).arg(data.m_qstr_recover_back)
                          .arg(data.m_qstr_recover_flash_text).arg(data.m_qstr_recover_flash_back)
                          .arg(data.m_nalarm_level));
    confdb_exec(qstr_condition.toStdString().c_str());
    return 0;
}

int CfdbService::getConfigData(int domianid, ConfigData &data)
{
    return 0;
}

int CfdbService::getColorData(int domianid, AlarmColor &data)
{
    return 0;
}

int CfdbService::updateConfigData(int domianid, QMap<int, ConfigData> &data)
{
    return 0;
}

bool CfdbService::isValidDomain(int domainid) const
{
    if (0 >= domainid)
    {
        return false;
    }

    return true;
}
