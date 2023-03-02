#include "xfs_media.h"
#include "service/printer/form/xfs_form_repository.h"

XFSMedia::XFSMedia(const QString &strName, XFSFormRepository *parent) : BlockElement{ strName, parent }
{
    setLogger(parent->logger());
    addElement(&m_oUnit);
    addElement(&m_strType);
    addElement(&m_oSize);
    addElement(&m_aPrintArea);
    addElement(&m_aRestricted);
    addElement(&m_iStaggering);
    addElement(&m_strFold);
    addElement(&m_iLines);
}

XFSMedia::~XFSMedia() { }
