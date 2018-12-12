#ifndef TRANSLATESERVICE_H
#define TRANSLATESERVICE_H
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>

class TransLateService
{
public:
    TransLateService();
    QString transMapToJson(const QMap<int,QString> &fields, const QMap<int,int> &policy);
    int transJsonToMap(QMap<int,QString> &field, QMap<int,int> &policy, const QString &json);
    QString transJsonValueToFieldName(const QString &value) const;
    int transJsonValueToPolicy(const QString &value) const;
};

#endif // TRANSLATESERVICE_H
