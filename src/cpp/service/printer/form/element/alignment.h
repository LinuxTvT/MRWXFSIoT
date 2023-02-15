#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "element.h"

namespace xfs_printer {

class Alignment : public Element
{
public:
    //    alignment
    //            Specifies the relative alignment of the form on the media and can be one of the following values:
    //• topLeft - The form is aligned relative to the top and left edges of the media.
    //• topRight - The form is aligned relative to the top and right edges of the media.
    //• bottomLeft - The form is aligned relative to the bottom and left edges of the media.
    //• bottomRight - The form is aligned relative to the bottom and right edges of the media
    inline static QHash<QString, QString> ALIGNMENT{ { "TOPLEFT", "topLeft" }, //
                                                     { "TOPRIGHT", "topRight" }, //
                                                     { "BOTTOMLEFT", "bottomLeft" }, //
                                                     { "BOTTOMRIGHT", "bottomRight" } };
    Alignment() : Element(Element::KW_ALIGNMENT, 3) { }
    inline const QString &alignment() const { return m_strAlignment; }
    inline int xOffset() const { return m_iXOffset; }
    inline int yOffset() const { return m_iYOffset; }

    virtual bool load(const QString &strPara) override;

private:
    QString m_strAlignment;
    int m_iXOffset;
    int m_iYOffset;
    bool setAlignment(const QString &strAlignment);

    // Element interface
public:
    virtual bool dump2Json(QJsonObject &jsonObject) override;
};
}
#endif // ALIGNMENT_H
