#include "device_worker_v3x.h"
#include "XFSPTR.H"
#include "device/v3x/device_worker_thread_v3x.h"
#include "qglobal.h"
#include "service/abstract_service.h"
#include "utils/xfs_utils.h"
#include "xfs_iot_standard.h"

bool DeviceWorkerV3x::loadXFSSDK()
{
    HRESULT l_hRes;
    qDebug("Start Up WOSA/XFS Manager");
    l_hRes = WFSStartUp(s_dwVersionsRequired, &s_stVersion);
    if (l_hRes != WFS_SUCCESS) {
        qDebug("Load WOSA/XFS Manager failed");
        qDebug("ATMC Exit");
        return false;
    } else {
        qDebug("Load WOSA/XFS Manager success: ");
    }

    /**
     * Create Application Handle for WOSA/XFS
     */
    l_hRes = WFSCreateAppHandle(&s_hXFSApp);
    if (l_hRes != WFS_SUCCESS) {
        qDebug("Create XFS application handle failed");
        return false;
    }
    return true;
}

DeviceWorkerV3x::DeviceWorkerV3x(const QString &strLogicalName, //
                                 const QString &strDeviceClass, //
                                 const QString &strFileConfig, //
                                 AbstractService *pService)
    : AbstractDeviceWorker{ QString("%1_%2").arg(strDeviceClass, strLogicalName), //
                            strFileConfig, //
                            pService, //
                            new DeviceWorkerThreadV3x(this) },
      m_strLogicalName(strLogicalName),
      m_strDeviceClass(strDeviceClass)
{
}

void DeviceWorkerV3x::onWndMessage(UINT uMsg, LPWFSRESULT lpResult)
{
    error(QString("UNHANDLE MESS: =[%1][%2]").arg(uMsg, lpResult->RequestID));
}

bool DeviceWorkerV3x::wfsOpen()
{
    log(QString("Open device: [class][logical name]=[%1][%2]").arg(this->deviceClass(), this->logicalName()));

    if (m_hService != 0) {
        warn("Device was opened (Service handle not NULL) -> close first");
        WFSClose(m_hService);
        m_hService = 0;
    }

    HRESULT l_hResult = WFSOpen(m_strLogicalName.toUtf8().data(), //
                                s_hXFSApp, //
                                m_strAppName.toUtf8().data(), //
                                m_dwTraceLevel, //
                                m_dwOpenTimeOut, //
                                m_dwSrvcVersionsRequired, //
                                &m_stSrvcVersion, //
                                &m_stSPIVersion, //
                                &m_hService);

    if (l_hResult == WFS_SUCCESS) {
        log("SPI Vesrion: " + QString(XfsUtils::strWFSVERSION(&m_stSPIVersion)));
        log("Srvc Vesrion: " + QString(XfsUtils::strWFSVERSION(&m_stSrvcVersion)));
    } else {
        error(QString("Open WOSA/XFS failed: %1").arg(l_hResult));
        return false;
    }
    return true;
}

HRESULT DeviceWorkerV3x::wfsGetInfo(DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT *lppResult)
{
    debug(QString("WFSGetInfo: Category [%1]").arg(dwCategory));
    return WFSGetInfo(m_hService, dwCategory, lpQueryDetails, dwTimeOut, lppResult);
}

HRESULT DeviceWorkerV3x::wfsAsyncExecute(DWORD dwCommand, //
                                         LPVOID lpCmdData, //
                                         DWORD dwTimeOut, //
                                         LPREQUESTID lpRequestID)
{
    debug(QString("WFSAsyncExecute: CMD [%1]").arg(dwCommand));
    return WFSAsyncExecute(m_hService, dwCommand, lpCmdData, dwTimeOut, m_hWnd, lpRequestID);
}

HRESULT DeviceWorkerV3x::wfsCancelAsyncRequest(REQUESTID RequestID)
{
    debug(QString("wfsCancelAsyncRequest: REQUESTID [%1]").arg(RequestID));
    return WFSCancelAsyncRequest(m_hService, RequestID);
}

bool DeviceWorkerV3x::cancelCommand(uint uiClientID, int iRequestID) { }

bool DeviceWorkerV3x::init()
{
    AbstractDeviceWorker::init();
    return wfsOpen();
}
