#include "device_worker_thread_v3x.h"
#include "XFSAPI.H"
#include "global_define.h"
#include "qthread.h"
#include "utils/xfs_utils.h"
#include <QCoreApplication>
#include <windows.h>

#define XFS_WND_CLASS_NAME (L"DUMMY_CLASS")
#define LOGER_NAME "XFS_EVENT"

MRWLogger *g_pXFSEventLogger = MRWLogger::createLogger(LOGER_NAME);

LRESULT DeviceWorkerThreadV3x::XFSMessageProc(HWND hwnd, UINT uMsg, LPWFSRESULT lpResult)
{
    if (HWND_HASH.contains(hwnd)) {
        g_pXFSEventLogger->d(QString("UKNOWN Device worker to process MESSAGE"));
    } else {
        DeviceWorkerV3x *l_pDeviceWorker = HWND_HASH.value(hwnd);
        l_pDeviceWorker->onWndMessage(uMsg, lpResult);
        if (lpResult != NULL) {
            WFSFreeResult(lpResult);
        }
    }
    return 0;
}

DeviceWorkerThreadV3x::DeviceWorkerThreadV3x(DeviceWorkerV3x *pWorker) : m_pWorker(pWorker) { }

LRESULT CALLBACK MainWndProc(HWND hwnd, // handle to window
                             UINT uMsg, // message identifier
                             WPARAM wParam, // first message parameter
                             LPARAM lParam) // second message parameter
{
    g_pXFSEventLogger->d(QString("Have MESSAGE [%1]").arg(uMsg));
    switch (uMsg) {
        case WFS_EXECUTE_COMPLETE:
        case WFS_EXECUTE_EVENT:
        case WFS_SERVICE_EVENT:
            return DeviceWorkerThreadV3x::XFSMessageProc(hwnd, uMsg, (LPWFSRESULT)lParam);
        case WM_CREATE:
            g_pXFSEventLogger->i("Windows WM_CREATE");
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        case WM_DESTROY:
            g_pXFSEventLogger->i("Windows WM_DESTROY");
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        default:
            if (uMsg > WM_USER) {
                g_pXFSEventLogger->i(QString("UNKNOWN XFS EVENT, MSG: 0x%1").arg(uMsg, 4, 16));
            }
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void DeviceWorkerThreadV3x::run()
{
    MSG Msg;
    WNDCLASSEX wx = {};
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = MainWndProc; // function which will handle messages
    wx.hInstance = GetModuleHandle(NULL);
    wx.lpszClassName = XFS_WND_CLASS_NAME;
    qDebug() << QString("XFSDeviceEventHandle , in thread id [0x%1]").arg((long long)QThread::currentThread(), 0, 16);

    g_pXFSEventLogger->i(QString("Run XFS_EVEN Handle [%1], in thread id [0x%2]")
                                 .arg(metaObject()->className())
                                 .arg((long long)QThread::currentThread(), 0, 16));

    g_pXFSEventLogger->i("Register windows class by RegisterClassEx");

    if (RegisterClassEx(&wx)) {
        g_pXFSEventLogger->i("Register windows class success");
        m_hWnd = CreateWindowEx(0, XFS_WND_CLASS_NAME, L"XFS Event Handle", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL,
                                NULL);
        if (m_hWnd != NULL) {
            g_pXFSEventLogger->i("Create Window success");
        } else {
            g_pXFSEventLogger->e("Create Window error");
        }
    } else {
        g_pXFSEventLogger->e("Register windows class error");
    }

    // emit started();

    if (m_hWnd == NULL) {
        return;
    } else {
        m_pWorker->setHWND(m_hWnd);
        HWND_HASH.insert(m_hWnd, m_pWorker);
    }

    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}
