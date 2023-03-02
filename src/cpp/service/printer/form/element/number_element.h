#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#define NUMBER_ELEMENT_NULL_VALUE -1

#include "element.h"
namespace xfs_printer {

class NumberElement : public Element
{
public:
    explicit NumberElement(const QString &strKeyWord, const QString &strJsonKey = QString());
    virtual ~NumberElement();
    virtual bool load(const QString &strPara) override;
    inline void setValue(int iVal) { m_iValue = iVal; }
    inline int value() const { return m_iValue; }
    inline bool isNull() const { return m_iValue == NUMBER_ELEMENT_NULL_VALUE; }

    virtual bool dump2Json(QJsonObject &jsonObject) override;

private:
    int m_iValue = NUMBER_ELEMENT_NULL_VALUE;
};

}
#endif // NUMBERELEMENT_H
