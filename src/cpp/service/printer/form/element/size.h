#ifndef SIZE_H
#define SIZE_H

#include "element.h"

namespace xfs_printer {

class Size : public Element
{
public:
    Size(bool isDump2Json = true) : Element(Element::KW_SIZE, 2), m_isDump2Json(isDump2Json) { }
    inline int height() const { return m_iHeight; }
    inline int width() const { return m_iWidth; }
    virtual bool load(const QString &strPara) override;

private:
    int m_iHeight = 0;
    int m_iWidth = 0;
    bool m_isDump2Json;

    // Element interface
public:
    virtual bool dump2Json(QJsonObject &jsonObject) override;
};

}
#endif // SIZE_H
