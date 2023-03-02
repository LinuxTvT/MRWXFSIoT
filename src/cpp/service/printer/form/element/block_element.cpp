#include "block_element.h"
#include "service/printer/form/xfs_form.h"

using namespace xfs_printer;

BlockElement::BlockElement(const QString &strName, QObject *parent) : SelfServiceObject{ parent, strName } { }

bool BlockElement::parse(TextLineProducer &lineProducer)
{
    if (lineProducer.nextLine() == "BEGIN") {
        while (lineProducer.nextLine() != "END") {
            QRegularExpressionMatch match = XFSForm::REGX_KEYWORD.match(lineProducer.currentLine());
            if (match.hasMatch()) {
                QString l_strKeyWord = match.captured(1);
                QString l_StrParameter = match.captured(3);
                if (loadElement(l_strKeyWord, l_StrParameter)) {
                    debug(QString("[%1] Load Element [%2] success").arg(name(), l_strKeyWord));
                } else {
                    error(QString("[%1] Load Element [%2] ERROR").arg(name(), l_strKeyWord));
                    return false;
                }
            } else {
                error(QString("[%1] Line invalid [%2]").arg(name(), lineProducer.currentLine()));
                return false;
            }
        }
    } else {
        error(QString("[%1] Require BEGIN at [%2]").arg(name(), lineProducer.currentLine()));
        return false;
    }
    return true;
}

bool BlockElement::dump2Json(QJsonObject &jsonObject) const
{
    for (auto itr = m_hElements.constBegin(); itr != m_hElements.constEnd(); itr++) {
        (*itr)->dump2Json(jsonObject);
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
