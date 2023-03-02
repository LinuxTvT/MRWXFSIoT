#ifndef STRINGCOMBINATIONELEMENT_H
#define STRINGCOMBINATIONELEMENT_H

#include "string_element.h"

namespace xfs_printer {

class StringCombinationElement : public StringElement
{
public:
    explicit StringCombinationElement(const QString &strKeyWord, //
                                      const QString &strDefaultValue = QString{}, //
                                      QHash<QString, QString> *pValues = nullptr, //
                                      const QString &strJsonKey = QString{});
    virtual ~StringCombinationElement();
    const QStringList &listValues() const { return m_strListValues; }
    virtual bool load(const QString &strPara) override;

private:
    QStringList m_strListValues;
};

}

#endif // STRINGCOMBINATIONELEMENT_H
