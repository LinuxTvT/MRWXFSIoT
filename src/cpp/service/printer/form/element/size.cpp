#include "size.h"
#include "service/printer/form/xfs_form.h"

bool Size::load(const QString &strPara)
{
    Element::load(strPara);
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() != 2) {
        return false;
    } else {
        m_iWidth = l_listPara[0].toInt();
        m_iHeight = l_listPara[1].toInt();
        return true;
    }
}

bool Size::dump2Json(QJsonObject &jsonObject)
{
    if (m_isDump2Json) {
        jsonObject["width"] = m_iWidth;
        jsonObject["height"] = m_iHeight;
    }
    return true;
}
