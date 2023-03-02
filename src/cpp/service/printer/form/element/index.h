#ifndef INDEX_H
#define INDEX_H

#include "element.h"

namespace xfs_printer {

class Index : public Element
{
public:
    explicit Index();
    virtual ~Index();
    inline int xOffset() const { return m_iXOffset; }
    inline int yOffset() const { return m_iYOffset; }
    inline int repeadCount() const { return m_iRepeatCount; }
    inline bool isNull() const { return m_iRepeatCount == 0; }
    inline bool isEmpty() const { return m_iRepeatCount == 0; }

private:
    int m_iRepeatCount;
    int m_iXOffset;
    int m_iYOffset;

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
    virtual bool dump2Json(QJsonObject &jsonObject) override;
};

}

#endif // INDEX_H
