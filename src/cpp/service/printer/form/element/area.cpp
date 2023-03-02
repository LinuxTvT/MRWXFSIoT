#include "area.h"
#include "service/printer/form/xfs_form.h"

using namespace xfs_printer;

Area::Area(const QString &strKeyWord) : Element(strKeyWord, 4) { }

Area::~Area() { }

bool Area::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() == 4) {
        m_iX = l_listPara[0].toInt();
        m_iY = l_listPara[1].toInt();
        m_iWidth = l_listPara[2].toInt();
        m_iHeight = l_listPara[3].toInt();
        return true;
    } else {
        return false;
    }
}
