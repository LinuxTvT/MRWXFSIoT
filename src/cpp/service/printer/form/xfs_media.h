#ifndef XFSMEDIA_H
#define XFSMEDIA_H

#include "service/printer/form/element/area.h"
#include "service/printer/form/element/block_element.h"
#include "service/printer/form/element/index.h"
#include "service/printer/form/element/number_element.h"
#include "service/printer/form/element/size.h"
#include "service/printer/form/element/string_element.h"
#include "service/printer/form/element/unit.h"
#include <QObject>

class XFSFormRepository;

class XFSMedia : public BlockElement
{
    Q_OBJECT

public:
    inline static QHash<QString, QString> TYPE{ { "GENERIC", "generic" }, //
                                                { "MULTIPART", "multipart" }, //
                                                { "PASSBOOK", "passbook" } };

    // foldType
    // Specified the type of fold for a media of type passbook as one of the following:
    //• none - Passbook has no fold.
    //• horizontal - Passbook has a horizontal fold.
    //• vertical - Passbook has a vertical fold

    inline static QHash<QString, QString> FOLD{ { "NONE", "none" }, //
                                                { "HORIZONTAL", "horizontal" }, //
                                                { "VERTICAL", "vertical" } };

public:
    explicit XFSMedia(const QString &strName, XFSFormRepository *parent);
    virtual ~XFSMedia();

private:
    StringElement m_strType{ Element::KW_TYPE };
    Unit m_oUnit;
    Size m_oSize;
    Area m_aPrintArea{ Element::KW_PRINTAREA };
    Area m_aRestricted{ Element::KW_RESTRICTED };
    NumberElement m_iStaggering{ Element::KW_STAGGERING };
    StringElement m_strFold{ Element::KW_FOLD, "none", &FOLD, "foldType" };
    NumberElement m_iLines{ Element::KW_LINES };
};

#endif // XFSMEDIA_H
