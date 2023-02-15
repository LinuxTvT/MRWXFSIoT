#include "xfs_field.h"
#include "service/printer/form/xfs_form.h"

XFSField::XFSField(const QString &strName, XFSForm *parent) : BlockElement{ strName, parent }
{
    this->setLogger(parent->logger());
    addElement(&m_oPosition);
    addElement(&m_strFollows);
    addElement(&m_strOverflow);
    addElement(&m_strVertical);
    addElement(&m_strHorizontal);
    addElement(&m_strInitialvalue);
    addElement(&m_oSize);
    addElement(&m_strType);
    addElement(&m_strScaling);
    addElement(&m_strClass);
}

bool XFSField::load(TextLineProducer &lineProducer)
{
    if (lineProducer.nextLine() == "BEGIN") {
        while (lineProducer.nextLine() != "END") {
            QRegularExpressionMatch match = XFSForm::REGX_KEYWORD.match(lineProducer.currentLine());
            if (match.hasMatch()) {
                QString l_strKeyWord = match.captured(1);
                QString l_StrParameter = match.captured(3);
                if (this->loadElement(l_strKeyWord, l_StrParameter)) {
                    // debug(QString("Load Element [%1] success").arg(l_strKeyWord));
                } else {
                    error(QString("Load Element [%1] ERROR").arg(l_strKeyWord));
                    return false;
                }
            } else {
                error(QString("Line invalid [%1]").arg(lineProducer.currentLine()));
                return false;
            }
        }
    } else {
        error(QString("Require BEGIN at [%1]").arg(lineProducer.currentLine()));
        return false;
    }
    return true;
}
