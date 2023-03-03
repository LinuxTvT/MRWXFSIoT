#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#define NUMBER_ELEMENT_NULL_VALUE 0

#include "element.h"
namespace xfs_printer {

class NumberElement : public Element
{
public:
    explicit NumberElement(const QString &strKeyWord, const QString &strJsonKey = QString());
    virtual ~NumberElement();

    inline void setValue(int iVal) { m_iValue = iVal; }
    inline int value() const { return m_iValue; }
    inline bool isNull() const { return m_iValue == NUMBER_ELEMENT_NULL_VALUE; }

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
    virtual bool dump2Json(QJsonObject &jsonObject) const override;

private:
    int m_iValue = NUMBER_ELEMENT_NULL_VALUE;
};

}
#endif // NUMBERELEMENT_H
