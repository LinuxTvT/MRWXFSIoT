#ifndef DEVICEWORKERV3X_H
#define DEVICEWORKERV3X_H

#include "XFSAPI.H"
#include "device/abstract_device_worker.h"
#include "runable_object.h"
#include <QObject>
#include <windows.h>

class AbstractService;

class DeviceWorkerV3x : public AbstractDeviceWorker
{
    Q_OBJECT

    // Static
private:
    inline static HAPP s_hXFSApp = NULL;
    inline static DWORD s_dwVersionsRequired = 0x00030003;
    inline static WFSVERSION s_stVersion;

public:
    static bool loadXFSSDK();

public:
    explicit DeviceWorkerV3x(const QString &strFileConfig, AbstractService *pService);

    virtual void onWndMessage(UINT uMsg, LPWFSRESULT lpResult);

    // XFS fuction
    /**
     * HRESULT extern WINAPI WFSOpen ( LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD
     * dwTraceLevel, DWORD dwTimeOut, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion,
     * LPWFSVERSION lpSPIVersion, LPHSERVICE lphService);
     * @brief wfsOpen
     * @return
     */
    bool wfsOpen();
    /**
     * HRESULT extern WINAPI WFSGetInfo ( HSERVICE hService, DWORD dwCategory, LPVOID
     *                          lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT * lppResult);
     */
    HRESULT wfsGetInfo(DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT *lppResult);

    /*
     * HRESULT extern WINAPI WFSExecute ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData,
     *                                    WORD dwTimeOut, LPWFSRESULT * lppResult);
     */
    bool wfsExecute(DWORD dwCommand, LPVOID lpCmdData = NULL);

    /**
    * @brief m_hService
    * HRESULT extern WINAPI WFSAsyncExecute ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData,
      DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);
    */

    bool wfsAsyncExecute(DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut = WFS_INDEFINITE_WAIT);

    /**
     * @brief m_hService
     * HRESULT extern WINAPI WFSCancelAsyncRequest ( HSERVICE hService, REQUESTID RequestID);
     */

    bool wfsCancelAsyncRequest(REQUESTID RequestID);

    inline const QString &logicalName() const { return m_strLogicalName; }
    inline const QString &deviceClass() const { return m_strDeviceClass; }

protected:
    HSERVICE m_hService = 0;
    REQUESTID m_asyncRequestID = 0;
    WFSVERSION m_stSrvcVersion;
    WFSVERSION m_stSPIVersion;
    DWORD m_dwResetCommand;
    DWORD m_dwCapabilitiesCategory;
    DWORD m_dwStatusCategory;
    LPWFSRESULT m_lpWFSResult = NULL;
    LPWFSRESULT m_lpWFSResultStatus = NULL;
    LPWFSRESULT m_lpWFSResultCapabilities = NULL;
    HRESULT m_hResult = WFS_SUCCESS;

    /**
     * Can configure member
     */
    DWORD m_dwGetInfoTimeOut = 300000;
    DWORD m_dwOpenTimeOut = 3000;
    DWORD m_dwTraceLevel = WFS_TRACE_ALL_API;
    DWORD m_dwSrvcVersionsRequired = 0x00030003;

    HWND m_hWnd = NULL;

private:
    void clearResult();
    QString m_strLogicalName;
    QString m_strDeviceClass;

    // AbstractDeviceWorker interface
public:
    virtual bool doCommand(XFSIoTCommandEvent *pCommandEvent) override;
    virtual bool cancelCommand(uint uiClientID, int iRequestID) override;
    virtual bool init() override;
};

#endif // DEVICEWORKERV3X_H
