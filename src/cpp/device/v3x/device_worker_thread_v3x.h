#ifndef DEVICEWORKERTHREADV3X_H
#define DEVICEWORKERTHREADV3X_H

#include "XFSAPI.H"
#include "device/abstract_device_worker.h"
#include "device/v3x/device_worker_v3x.h"
#include <QObject>
#include <QThread>
#include <windows.h>

class DeviceWorkerThreadV3x : public QThread
{
    Q_OBJECT

private:
    inline static QHash<HWND, DeviceWorkerV3x *> HWND_HASH;

public:
    static LRESULT XFSMessageProc(HWND hwnd, UINT uMsg, LPWFSRESULT lpResult);

public:
    explicit DeviceWorkerThreadV3x(DeviceWorkerV3x *pWorker);
    inline HWND hWnd() const { return m_hWnd; };

private:
    DeviceWorkerV3x *m_pWorker;

    // QThread interface
protected:
    virtual void run() override;

private:
    HWND m_hWnd = NULL;
};

#endif // DEVICEWORKERTHREADV3X_H
