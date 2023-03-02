#include "xfs_frame.h"
#include "service/printer/form/xfs_form.h"

XFSFrame::XFSFrame(const QString &strName, XFSForm *parent) : BlockElement{ strName, parent }
{
    addElement(&m_oPosition);
    addElement(&m_oSize);
    addElement(&m_strFrames);
    addElement(&m_strColor);
    addElement(&m_strFillColor);
    addElement(&m_strFillStyle);
    addElement(&m_strTile);
}
