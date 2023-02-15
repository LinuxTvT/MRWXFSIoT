#ifndef POSITION_H
#define POSITION_H

#include "element.h"

namespace xfs_printer {

class Position : public Element
{
public:
    Position() : Element(Element::KW_POSITION, 2) { }
    Position(const Position &other);
    virtual bool load(const QString &strPara) override;
    bool operator<(Position const &obj) const;
    inline int x() const { return m_iX; }
    inline int y() const { return m_iY; }
    inline int z() const { return m_iZ; }

private:
    int m_iX;
    int m_iY;
    int m_iZ;
};

}
#endif // POSITION_H
