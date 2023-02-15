#ifndef XFS_IOT_STANDARD_H
#define XFS_IOT_STANDARD_H

#include "interface/xfs_iot_interface.h"
#include "selfservice_object.h"

#define JsonValue(x, y) inline static const QString JV_##x##_##y = QStringLiteral(#y);

class XFSIoTStandard : public SelfServiceObject
{
public:
    /******************************************************************************************************************
     * 2.3.2 Header Definition
     * {
     *     "header": {
     *         "type": "command",
     *         "name": "Common.Status",
     *         "requestId": 12345
     *     },
     *     "payload": {
     *     }
     * }
     ******************************************************************************************************************/
    inline static const QString JK_HEADER = QStringLiteral("header");
    inline static const QString JK_TYPE = QStringLiteral("type");
    inline static const QString JK_NAME = QStringLiteral("name");
    inline static const QString JK_REQUEST_ID = QStringLiteral("requestId");

    inline static const QString JK_PAYLOAD = QStringLiteral("payload");
    inline static const QString JK_COMPLETION_CODE = QStringLiteral("completionCode");

    /* 2.4.2 Acknowledge Messages */
    inline static const QString JK_STATUS = QStringLiteral("status");
    inline static const QString JK_ERROR_DESCRIPTION = QStringLiteral("errorDescription");

    inline static const QString JK_SEVICE_URI = QStringLiteral("serviceURI");
    inline static const QString JK_SERVICES = QStringLiteral("services");

    // Value
    // Message type
    inline static const QString JV_UNDEFINE = QStringLiteral("undefine");
    inline static const QString JV_TYPE_COMMAND = QStringLiteral("command");
    inline static const QString JV_TYPE_ACKNOWLEDGE = QStringLiteral("acknowledge");
    inline static const QString JV_TYPE_EVENT = QStringLiteral("event");
    inline static const QString JV_TYPE_COMPLETION = QStringLiteral("completion");
    inline static const QString JV_TYPE_UNSOLICITED = QStringLiteral("unsolicited");

    inline static const QString JV_INVALID_MESSAGE = QStringLiteral("invalidMessage");
    inline static const QString JV_INVALID_REQUEST_ID = QStringLiteral("invalidRequestID");
    inline static const QString JV_INVALID_COMMAND = QStringLiteral("invalidCommand");
    inline static const QString JV_UNSUPPORTED_COMMAND = QStringLiteral("unsupportedCommand");
    inline static const QString JV_INVALID_DATA = QStringLiteral("invalidData");
    inline static const QString JV_COMMAND_ERROR_CODE = QStringLiteral("commandErrorCode");
    inline static const QString JV_CANCELED = QStringLiteral("canceled");

    inline static const QString JV_INTERFACE_NAME_COMMON = QStringLiteral("Common");

    //    fields/exampleProperty1/type
    //                Specifies the type of field as one of the following:
    //• text - The field is a text field.
    //• micr - The field is a Magnetic Ink Character Recognition field.
    //• ocr - The field is an Optical Character Recognition field.
    //• msf - The field is a Magnetic Stripe Facility field.
    //• barcode - The field is a barcode field.
    //• graphic - The field is a Graphic field.
    //• pagemark - The field is a Page Mark field.

    JsonValue(FIELD_TYPE, text);
    JsonValue(FIELD_TYPE, micr);
    JsonValue(FIELD_TYPE, ocr);
    JsonValue(FIELD_TYPE, msf);
    JsonValue(FIELD_TYPE, barcode);
    JsonValue(FIELD_TYPE, graphic);
    JsonValue(FIELD_TYPE, pagemark);

    //    fields/exampleProperty1/class
    //            Specifies the class of the field as one of the following:
    //• static - The field data cannot be set by the application.
    //• optional - The field data can be set by the application.
    //• required - The field data must be set by the application.

    JsonValue(FIELD_CLASS, static);
    JsonValue(FIELD_CLASS, optional);
    JsonValue(FIELD_CLASS, required);

    //     fields/exampleProperty1/access
    // Specifies the field access as one of the following:
    //• read - The field is used for input.
    //• write - The field is used for output.
    //• readWrite - The field is used for both input and output.
    JsonValue(FIELD_ACCESS, read);
    JsonValue(FIELD_ACCESS, write);
    JsonValue(FIELD_ACCESS, readWrite);

    //     fields/exampleProperty1/overflow
    // Specifies how an overflow of field data should be handled as one of the following:
    //• terminate - Return an error and terminate printing of the form.
    //• truncate - Truncate the field data to fit in the field.
    //• bestFit - Fit the text in the field.
    //• overwrite - Print the field data beyond the extents of the field boundary.
    //• wordWrap - If the field can hold more than one line the text is wrapped around. Wrapping is performed,
    // where possible, by splitting the line on a space character or a hyphen character or any other character
    // which is used to join two words together.
    JsonValue(FIELD_OVERFLOW, terminate);
    JsonValue(FIELD_OVERFLOW, truncate);
    JsonValue(FIELD_OVERFLOW, bestFit);
    JsonValue(FIELD_OVERFLOW, overwrite);
    JsonValue(FIELD_OVERFLOW, wordWrap);

    // Puslisher
    inline static const QString JV_SUCCESS = QStringLiteral("success");

    // cuntion
    static void buildPayloadCompletion(QJsonObject &joPayload, const QString &strCode = JV_SUCCESS,
                                       const QString &strDesc = QString());

    static XFSIoTInterface *getInterface(const QString &strName);

private:
    explicit XFSIoTStandard() { }
    virtual ~XFSIoTStandard() { }
};

#endif // XFS_IOT_STANDARD_H
