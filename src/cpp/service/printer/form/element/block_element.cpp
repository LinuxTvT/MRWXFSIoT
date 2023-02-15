#include "block_element.h"
#include "service/printer/form/xfs_form.h"

using namespace xfs_printer;

BlockElement::BlockElement(const QString &strName, QObject *parent) : SelfServiceObject{ parent, strName } { }

bool BlockElement::dump2Json(QJsonObject &jsonObject) const
{
    for (auto itr = m_hElements.constBegin(); itr != m_hElements.constEnd(); itr++) {
        if ((*itr)->isLoaded()) {
            (*itr)->dump2Json(jsonObject);
        }
    }
    return true;
}

void BlockElement::addElement(Element *e)
{
    m_hElements.insert(e->keyWord(), e);
}

bool BlockElement::loadElement(const QString &strName, const QString &strParemetter)
{
    if (m_hElements.contains(strName)) {
        return m_hElements.value(strName)->load(strParemetter);
    } else {
        return false;
    }
}
