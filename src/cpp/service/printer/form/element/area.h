#ifndef AREA_H
#define AREA_H

#include "element.h"

namespace xfs_printer {

class Area : public Element
{
public:
    explicit Area(const QString &strKeyWord);
    virtual ~Area();

private:
    int m_iX;
    int m_iY;
    int m_iWidth;
    int m_iHeight;

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
};

}

#endif // AREA_H
