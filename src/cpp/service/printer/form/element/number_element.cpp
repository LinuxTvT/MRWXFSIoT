#include "number_element.h"

using namespace xfs_printer;

NumberElement::NumberElement(const QString &strKeyWord, const QString &strJsonKey)
    : Element{ strKeyWord, 1, strJsonKey }
{
}

NumberElement::~NumberElement() { }

bool NumberElement::load(const QString &strPara)
{
    Element::load(strPara);
    bool l_isOK;
    m_iValue = strPara.toInt(&l_isOK);
    return l_isOK;
}

bool NumberElement::dump2Json(QJsonObject &jsonObject)
{
    if (!this->jsonKey().isEmpty()) {
        jsonObject[this->jsonKey()] = m_iValue;
    }
    return true;
}
