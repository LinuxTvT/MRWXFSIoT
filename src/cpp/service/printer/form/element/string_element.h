#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H

#include "element.h"
namespace xfs_printer {

class StringElement : public Element
{
public:
    explicit StringElement(const QString &strKeyWord, //
                           const QString &strDefaultValue = QString{}, //
                           QHash<QString, QString> *pValues = nullptr, //
                           const QString &strJsonKey = QString{});

    virtual bool load(const QString &strPara) override;
    inline const QString &value() const { return m_strValue; }
    inline bool is(const QString &str) const { return (m_strValue == str); }

protected:
    inline QHash<QString, QString> *defineValues() const { return m_pValues; }
    inline void setValue(const QString &strValue) { m_strValue = strValue; }

private:
    QString m_strValue;
    QHash<QString, QString> *m_pValues = nullptr;

    // Element interface
public:
    virtual bool dump2Json(QJsonObject &jsonObject) override;
};

}
#endif // STRINGELEMENT_H
