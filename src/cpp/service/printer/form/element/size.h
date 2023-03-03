#ifndef SIZE_H
#define SIZE_H

#include "element.h"
#include "qsize.h"

namespace xfs_printer {

class Size : public Element
{
public:
    Size(bool isDump2Json = true);
    virtual ~Size();
    inline const QSize &qSize() const { return m_qSize; }

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
    virtual bool dump2Json(QJsonObject &jsonObject) const override;

private:
    QSize m_qSize;
    bool m_isDump2Json;
};

}
#endif // SIZE_H
