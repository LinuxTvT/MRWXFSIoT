#include "position.h"
#include "service/printer/form/xfs_form.h"

Position::Position(const Position &other) : Position{}
{
    m_iX = other.m_iX;
    m_iY = other.m_iY;
    m_iZ = other.m_iZ;
}

bool Position::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() == 2) {
        m_iX = l_listPara[0].toInt();
        m_iY = l_listPara[1].toInt();
        return true;
    } else if (l_listPara.size() == 3) {
        m_iX = l_listPara[0].toInt();
        m_iY = l_listPara[1].toInt();
        m_iZ = l_listPara[2].toInt();
        return true;
    } else {
        return false;
    }
}

bool Position::operator<(const Position &obj) const
{
    if (m_iX < obj.m_iX) {
        return true;
    } else {
        return (m_iY < obj.m_iY);
    }
}
