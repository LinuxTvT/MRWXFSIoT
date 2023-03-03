#include "xfs_frame.h"
#include "service/printer/form/xfs_form.h"

XFSFrame::XFSFrame(const QString &strName, XFSForm *parent) //
    : BlockElement{ strName, parent }, m_pForm(parent)
{
    addElement(&m_oPosition);
    addElement(&m_oSize);
    addElement(&m_strFrames);
    addElement(&m_strColor);
    addElement(&m_strFillColor);
    addElement(&m_strFillStyle);
    addElement(&m_strTitle);
}

bool XFSFrame::rebuild()
{
    if (m_pForm == nullptr) {
        error(QString("Frame [%1] form parrent is null").arg(name()));
        return false;
    } else {
        if (!m_strFrames.isEmpty()) {
            XFSField *l_pField = m_pForm->field(m_strFrames.value());
            if (l_pField == nullptr) {
                error(QString("Can not find field [%1] for frame [%2]") //
                              .arg(m_strFrames.value(), name()));
                return false;
            } else {
                setContentField(l_pField);
            }
        }
        if (!m_strTitle.isEmpty()) {
            XFSField *l_pField = m_pForm->field(m_strTitle.value());
            if (l_pField == nullptr) {
                error(QString("Can not find field [%1] for tile of frame [%2]") //
                              .arg(m_strTitle.value(), name()));
                return false;
            } else {
                setTitleField(l_pField);
            }
        }
    }
    return true;
}

void XFSFrame::setContentField(XFSField *pContentField)
{
    m_pContentField = pContentField;
    pContentField->setFrame(this);
}

void XFSFrame::setTitleField(XFSField *pTitleField)
{
    m_pTitleField = pTitleField;
    pTitleField->setTileOfFrame(this);
}
