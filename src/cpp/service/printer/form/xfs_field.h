#ifndef XFSFIELD_H
#define XFSFIELD_H

#include <QObject>
#include "qjsonobject.h"
#include "selfservice_object.h"
#include "service/printer/form/element/position.h"
#include "service/printer/form/element/size.h"
#include "service/printer/form/element/string_element.h"
#include "service/printer/form/text_line_producer.h"
#include "service/printer/form/element/block_element.h"

class XFSForm;

class XFSField : public BlockElement
{
    Q_OBJECT
public:
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

    inline static QHash<QString, QString> SCALING{ { "BESTFIT", "bestfit" }, //
                                                   { "ASIS", "asis" }, //
                                                   { "MAINTAINASPECT", "maintainAspect" } };

    inline static QHash<QString, QString> CLASS{ { "OPTIONAL", "optional" }, //
                                                 { "STATIC", "static" }, //
                                                 { "REQUIRED", "required" } };

    explicit XFSField(const QString &strName, XFSForm *parent);
    virtual ~XFSField(){};
    bool load(TextLineProducer &lineProducer) override;

    inline const Position &position() const { return m_oPosition; }
    inline const Size &size() const { return m_oSize; }
    inline const StringElement &overflow() const { return m_strOverflow; }
    inline const QString &initialvalue() const { return m_strInitialvalue.value(); }
    inline const StringElement &fieldClass() const { return m_strClass; }

private:
    Position m_oPosition;
    StringElement m_strFollows{ Element::KW_FOLLOWS };
    StringElement m_strOverflow{ Element::KW_OVERFLOW, "overflow", &OVERFLOW };
    StringElement m_strVertical{ Element::KW_VERTICAL, &VERTICAL };
    StringElement m_strHorizontal{ Element::KW_HORIZONTAL, &HORIZONTAL };
    StringElement m_strInitialvalue{ Element::KW_INITIALVALUE, "initialValue" };
    StringElement m_strClass{ Element::KW_CLASS, "optional", &CLASS };
    // HEADER
    // FOOTER
    // SIDE
    Size m_oSize{ false };

    // INDEX
    StringElement m_strType{ Element::KW_TYPE, "type", &TYPE };
    StringElement m_strScaling{ Element::KW_SCALING, &SCALING };
};

#endif // XFSFIELD_H
