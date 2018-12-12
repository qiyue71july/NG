#include "translateservice.h"
#include <QJsonObject>
#include <QByteArray>
#include <QJsonArray>
#include <QDebug>

TransLateService::TransLateService()
{

}

QString TransLateService::transMapToJson(const QMap<int, QString> &fields, const QMap<int, int> &policy)
{
    if(fields.isEmpty() || policy.isEmpty())
    {
        return "";
    }

    QJsonObject qjsonobject_info;
    QMap<int,QString>::const_iterator iterator_field = fields.begin();
    int npolicy_opt = 1;
    while (fields.end() != iterator_field)
    {
        if (policy.contains(iterator_field.key()))
        {
            npolicy_opt = policy.value(iterator_field.key());
        }
        else
        {
            npolicy_opt = 1;
        }
        qjsonobject_info.insert(QString::number(iterator_field.key()),QString("%1,%2").arg(iterator_field.value()).arg(policy.value(iterator_field.key())));
        ++iterator_field;
    }

    QJsonDocument j_document;
    j_document.setObject(qjsonobject_info);    
    QString qstr_json;
    qstr_json = QString::fromStdString(j_document.toJson(QJsonDocument::Compact).toStdString());
    return qstr_json;
}

int TransLateService::transJsonToMap(QMap<int, QString> &field, QMap<int, int> &policy, const QString &json)
{
    QByteArray arrary_temp = QByteArray::fromStdString(json.toStdString());
    QJsonObject qjsonobject_source;
    qjsonobject_source = QJsonDocument::fromJson(arrary_temp).object();
    QJsonObject::iterator iterator_json = qjsonobject_source.begin();
    while(qjsonobject_source.end() != iterator_json)
    {
        field.insert(iterator_json.key().toInt(),transJsonValueToFieldName(iterator_json.value().toString()));
        policy.insert(iterator_json.key().toInt(),transJsonValueToPolicy(iterator_json.value().toString()));
        ++iterator_json;
    }
    return 0;
}

QString TransLateService::transJsonValueToFieldName(const QString &value) const
{
    if(value.isEmpty())
        return "";
    int start = 0;
    int seek = value.indexOf(",",start);
    QString qstr_name;
    qstr_name.clear();
    while(--seek + 1)
    {
        qstr_name.append(value.at(start));
        ++start;
    }

    return qstr_name;
}

int TransLateService::transJsonValueToPolicy(const QString &value) const
{
    if(value.isEmpty())
        return -1;

    int count = value.count();
    int start = value.indexOf(",",0) + 1;
    int seek = count - start;
    QString qstr_number;
    qstr_number.clear();
    while(--seek + 1)
    {
        qstr_number.append(value.at(start));
        ++start;
    }
    if(qstr_number.isEmpty())
        return -1;

    return qstr_number.toInt();
}
