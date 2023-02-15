#include "text_line_producer.h"
#include "service/printer/form/xfs_form.h"

QString &TextLineProducer::nextLine()
{
    if (m_pStream->atEnd()) {
        m_strCurLine = QString();
    } else {
        m_strCurLine = m_pStream->readLine();
        // Remove comment
        int l_iIdxOfComment = m_strCurLine.indexOf("//");
        if (l_iIdxOfComment != -1) {
            m_strCurLine.truncate(l_iIdxOfComment);
        }
        m_strCurLine = m_strCurLine.simplified();
        if (m_strCurLine.endsWith('\\')) {
            m_strCurLine.append(nextLine());
        }
        if (m_strCurLine.isEmpty()) {
            m_strCurLine = nextLine();
        }
    }
    return m_strCurLine;
}
