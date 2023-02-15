#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H

#include "element.h"
namespace xfs_printer {

class StringElement : public Element
{
public:
    StringElement(const QString &strKeyWord, const QString &strJsonKey = QString{},
                  QHash<QString, QString> *pValues = nullptr);
    StringElement(const QString &strKeyWord, QHash<QString, QString> *pValues);

    virtual bool load(const QString &strPara) override;
    inline const QString &value() const { return m_strValue; }
    inline bool is(const QString &str) const { return (m_strValue == str); }

private:
    QString m_strValue;
    QHash<QString, QString> *m_pValues = nullptr;

    // Element interface
public:
    virtual bool dump2Json(QJsonObject &jsonObject) override;
};

}
#endif // STRINGELEMENT_H
