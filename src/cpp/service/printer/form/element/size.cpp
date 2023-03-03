#include "size.h"
#include "service/printer/form/xfs_form.h"

Size::Size(bool isDump2Json) //
    : Element(Element::KW_SIZE, 2), //
      m_isDump2Json(isDump2Json)
{
}

Size::~Size() { }

bool Size::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() != 2) {
        return false;
    } else {
        m_qSize.setWidth(l_listPara[0].toInt());
        m_qSize.setHeight(l_listPara[1].toInt());
        return true;
    }
}

bool Size::dump2Json(QJsonObject &jsonObject) const
{
    if (m_isDump2Json) {
        jsonObject["width"] = m_qSize.width();
        jsonObject["height"] = m_qSize.height();
    }
    return true;
}
