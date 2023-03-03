#ifndef AREA_H
#define AREA_H

#include "element.h"
#include <QRect>

namespace xfs_printer {

class Area : public Element
{
public:
    explicit Area(const QString &strKeyWord);
    virtual ~Area();

private:
    QRect m_qRect;

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
};

}

#endif // AREA_H
