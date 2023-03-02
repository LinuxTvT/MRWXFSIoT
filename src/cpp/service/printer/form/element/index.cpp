#include "index.h"
#include "service/printer/form/xfs_form.h"

using namespace xfs_printer;

Index::Index() : Element(Element::KW_INDEX, 3)
{
    m_iRepeatCount = 0;
}

Index::~Index() { }

bool Index::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() == 3) {
        m_iRepeatCount = l_listPara[0].toInt();
        m_iXOffset = l_listPara[1].toInt();
        m_iYOffset = l_listPara[2].toInt();
        return true;
    } else {
        return false;
    }
}

bool Index::dump2Json(QJsonObject &jsonObject)
{
    jsonObject["indexCount"] = m_iRepeatCount;
    return true;
}
