#ifndef BLOCKELEMENT_H
#define BLOCKELEMENT_H

#include "element.h"
#include "service/printer/form/text_line_producer.h"
#include <QObject>

using namespace xfs_printer;

class BlockElement : public SelfServiceObject
{
    Q_OBJECT

public:
    explicit BlockElement(const QString &strName, QObject *parent = nullptr);
    virtual ~BlockElement(){};
    virtual bool parse(TextLineProducer &lineProducer);
    virtual bool dump2Json(QJsonObject &jsonObject) const;

protected:
    void addElement(Element *e);
    bool loadElement(const QString &strName, const QString &strParemetter);

private:
    QHash<QString, Element *> m_hElements;
};

#endif // BLOCKELEMENT_H
