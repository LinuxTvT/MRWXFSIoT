#ifndef XFSFIELD_H
#define XFSFIELD_H

#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qnamespace.h"
#include "selfservice_object.h"
#include "service/printer/form/element/block_element.h"
#include "service/printer/form/element/index.h"
#include "service/printer/form/element/number_element.h"
#include "service/printer/form/element/position.h"
#include "service/printer/form/element/size.h"
#include "service/printer/form/element/string_combination_element.h"
#include "service/printer/form/element/string_element.h"
#include "service/printer/form/text_line_producer.h"
#include "service/printer/form/xfs_frame.h"
#include <QObject>
#include <QRect>

class XFSForm;

class XFSField : public BlockElement
{
    Q_OBJECT
public:
    inline static QString REGX_FIELD_INDEX_KEY_FORMAT{ "(?:^%1\\[)(\\d+)(?:\\]$)" };
    inline static QRegularExpression REGX_FIELD_INDEX{ "(?:^\\w+\\[)(\\d+)(?:\\]$)" };

    //    fields/exampleProperty1/type
    //    Specifies the type of field as one of the following:
    //• text - The field is a text field.
    //• micr - The field is a Magnetic Ink Character Recognition field.
    //• ocr - The field is an Optical Character Recognition field.
    //• msf - The field is a Magnetic Stripe Facility field.
    //• barcode - The field is a barcode field.
    //• graphic - The field is a Graphic field.
    //• pagemark - The field is a Page Mark field.
    inline static QHash<QString, QString> TYPE{ { "TEXT", "text" }, //
                                                { "MICR", "micr" }, //
                                                { "OCR", "ocr" }, //
                                                { "MSF", "msf" }, //
                                                { "BARCODE", "barcode" }, //
                                                { "GRAPHIC", "graphic" }, //
                                                { "PAGEMARK", "pagemark" } };
    // fields/exampleProperty1/overflow
    // Specifies how an overflow of field data should be handled as one of the following:
    //• terminate - Return an error and terminate printing of the form.
    //• truncate - Truncate the field data to fit in the field.
    //• bestFit - Fit the text in the field.
    //• overwrite - Print the field data beyond the extents of the field boundary.
    //• wordWrap - If the field can hold more than one line the text is wrapped around. Wrapping is performed,
    //             where possible, by splitting the line on a space character or a hyphen character or any other
    //             character which is used to join two words together.
    inline static QHash<QString, QString> OVERFLOW{ { "TERMINATE", "terminate" }, //
                                                    { "TRUNCATE", "truncate" }, //
                                                    { "BESTFIT", "bestFit" }, //
                                                    { "OVERWRITE", "overwrite" }, //
                                                    { "WORDWRAP", "wordWrap" } };

    inline static QHash<QString, QString> VERTICAL{ { "BOTTOM", "bottom" }, //
                                                    { "CENTER", "center" }, //
                                                    { "TOP", "top" } };

    inline static QHash<QString, QString> HORIZONTAL{ { "LEFT", "left" }, //
                                                      { "RIGHT", "right" }, //
                                                      { "CENTER", "center" }, //
                                                      { "JUSTIFY", "justify" } };
    inline static QHash<QString, int> HORIZONTAL_QT{ { "left", Qt::AlignLeft }, //
                                                     { "right", Qt::AlignRight }, //
                                                     { "center", Qt::AlignHCenter }, //
                                                     { "justify", Qt::AlignJustify } };

    inline static QHash<QString, QString> SCALING{ { "BESTFIT", "bestfit" }, //
                                                   { "ASIS", "asis" }, //
                                                   { "MAINTAINASPECT", "maintainAspect" } };

    inline static QHash<QString, QString> CLASS{ { "OPTIONAL", "optional" }, //
                                                 { "STATIC", "static" }, //
                                                 { "REQUIRED", "required" },
                                                 { "TIME", "time" } };

    inline static QHash<QString, QString> ACCESS{ { "WRITE", "write" }, //
                                                  { "READ", "read" }, //
                                                  { "READWRITE", "readWrite" } };

    inline static QHash<QString, QString> STYPE{ { "OPTIONAL", "optional" }, //
                                                 { "STATIC", "static" }, //
                                                 { "REQUIRED", "required" } };

    explicit XFSField(const QString &strName, XFSForm *parent);
    virtual ~XFSField(){};

    inline const QRegularExpression &fieldNameRegularExpression() const { return m_regxIndexFieldNameFilter; };

    QString checkFieldValue(const QJsonValue jvValue) const;
    QString checkFieldValue(const QJsonObject joFieldsPayload) const;
    QString printValue(const QJsonObject joFieldsPayload) const;
    int printValuesList(const QJsonObject joFieldsPayload, QMap<int, QString> &valuesList) const;

    inline const Position &position() const { return m_oPosition; }
    inline const Size &size() const { return m_oSize; }
    inline const Index &index() const { return m_oIndex; }
    inline bool isIndexField() const { return (m_oIndex.repeadCount() > 0); }
    inline const StringElement &overflow() const { return m_strOverflow; }
    inline const QString &initialvalue() const { return m_strInitialvalue.value(); }
    inline const StringElement &fieldClass() const { return m_strClass; }
    inline const StringElement &type() const { return m_strType; }
    inline const NumberElement &pointSize() const { return m_iPointSize; }
    inline const StringCombinationElement &style() const { return m_strStyle; }
    inline QStringList listStyles() const { return m_strStyle.value().split('|'); }
    inline const StringElement &follows() const { return m_strFollows; }
    inline int qtHAlign() const { return HORIZONTAL_QT.value(m_strHorizontal.value(), Qt::AlignLeft); }
    void setFrame(XFSFrame *pFrame);
    void setTileOfFrame(XFSFrame *pFrame);
    void setFieldFollows(XFSField *pField);
    const XFSField *nextFollows() const { return m_pNextFollows; }
    const XFSField *previousFollows() const { return m_pPreviousFollows; }

    inline QRect rect() const { return QRect{ m_oPosition.qPoint(), m_oSize.qSize() }; }
    inline QRect rect(int idx) const { return rect().translated(idx * m_oIndex.xOffset(), idx * m_oIndex.yOffset()); }
    inline const XFSForm *form() const { return m_pForm; }

private:
    Position m_oPosition;
    StringElement m_strFollows{ Element::KW_FOLLOWS };
    StringElement m_strOverflow{ Element::KW_OVERFLOW, "terminate", &OVERFLOW, "overflow" };
    // STYLE
    StringCombinationElement m_strStyle{ Element::KW_STYLE };

    StringElement m_strVertical{ Element::KW_VERTICAL, "bottom", &VERTICAL };
    StringElement m_strHorizontal{ Element::KW_HORIZONTAL, "left", &HORIZONTAL };
    StringElement m_strInitialvalue{ Element::KW_INITIALVALUE, QString{}, nullptr, "initialValue" };
    StringElement m_strClass{ Element::KW_CLASS, "optional", &CLASS, "class" };
    // ACCESS
    StringElement m_strAccess{ Element::KW_ACCESS, "write", &ACCESS, "access" };

    // HEADER
    // FOOTER
    // SIDE
    Size m_oSize{ false };

    // INDEX
    Index m_oIndex;
    StringElement m_strType{ Element::KW_TYPE, "text", &TYPE, "type" };
    StringElement m_strScaling{ Element::KW_SCALING, "bestfit", &SCALING };
    StringElement m_strFont{ Element::KW_FONT };
    NumberElement m_iPointSize{ Element::KW_POINTSIZE };
    // CPI
    NumberElement m_iCPI{ Element::KW_CPI };
    // LPI
    NumberElement m_iLPI{ Element::KW_LPI };

    XFSForm *m_pForm = nullptr;
    XFSFrame *m_pFrame = nullptr;
    XFSFrame *m_pTitleOfFrame = nullptr;
    XFSField *m_pNextFollows = nullptr;
    XFSField *m_pPreviousFollows = nullptr;
    QRegularExpression m_regxIndexFieldNameFilter;
};

#endif // XFSFIELD_H
