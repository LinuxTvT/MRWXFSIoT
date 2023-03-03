#ifndef INDEX_H
#define INDEX_H

#include "element.h"
#include <QPoint>

namespace xfs_printer {

// fields/exampleProperty1/indexCount
//     Specifies the number of entries for an index field.
//     A value of 0 indicates that this field is not an index field.
//     Index fields are typically used to present information in a tabular fashion.
//     Property value constraints:
//     minimum: 0

class Index : public Element
{
public:
    explicit Index();
    virtual ~Index();

    inline int xOffset() const { return m_pointOffset.x(); }
    inline int yOffset() const { return m_pointOffset.y(); }
    inline int repeadCount() const { return m_iRepeatCount; }
    inline const QPoint &offset() const { return m_pointOffset; }
    inline bool isZero() const { return m_iRepeatCount == 0; }

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
    virtual bool dump2Json(QJsonObject &jsonObject) const override;

private:
    int m_iRepeatCount = 0;
    QPoint m_pointOffset{ 0, 0 };
};

}

#endif // INDEX_H
