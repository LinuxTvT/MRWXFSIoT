#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#include "element.h"
namespace xfs_printer {

class NumberElement : public Element
{
public:
    explicit NumberElement(const QString &strKeyWord, const QString &strJsonKey = QString())
        : Element{ strKeyWord, strJsonKey }
    {
    }
    virtual bool load(const QString &strPara) override;
    inline void setValue(int iVal) { m_iValue = iVal; }
    inline int value() const { return m_iValue; }

    virtual bool dump2Json(QJsonObject &jsonObject) override;

private:
    int m_iValue;
};

}
#endif // NUMBERELEMENT_H
