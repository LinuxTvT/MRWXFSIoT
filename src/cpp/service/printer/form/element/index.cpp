#include "index.h"
#include "service/printer/form/xfs_form.h"

using namespace xfs_printer;

Index::Index() : Element(Element::KW_INDEX, 3) { }

Index::~Index() { }

bool Index::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() == 3) {
        m_iRepeatCount = l_listPara[0].toInt();
        m_pointOffset.setX(l_listPara[1].toInt());
        m_pointOffset.setY(l_listPara[2].toInt());
        return true;
    } else {
        return false;
    }
}

bool Index::dump2Json(QJsonObject &jsonObject) const
{
    jsonObject["indexCount"] = m_iRepeatCount;
    return true;
}
