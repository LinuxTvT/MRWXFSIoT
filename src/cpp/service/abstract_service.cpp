#include "abstract_service.h"
#include "device/device_worker.h"
#include "device/device_worker_thread.h"
#include "qjsonarray.h"
#include "qjsonvalue.h"
#include "qthread.h"
#include "service/service_endpoint.h"
#include "utils/json_utils.h"
#include "xfs_iot_standard.h"
#include <QCoreApplication>

InterfaceRepository *AbstractService::s_pInterfaceRepository = nullptr;

AbstractService::AbstractService(const QString &strName, //
                                 const QString &strFileConfig)
    : RunableObject(strName, strFileConfig, nullptr)
{
}

AbstractService::~AbstractService() { }

void AbstractService::setInterfaceRepository(InterfaceRepository *pInterfaceRepository)
{
    if (s_pInterfaceRepository != nullptr) {
        s_pInterfaceRepository->deleteLater();
    }
    s_pInterfaceRepository = pInterfaceRepository;
}

void AbstractService::Common_Capabilities(XFSIoTCommandEvent *pEvent)
{
    notifyCompletion(pEvent, XFSIoTStandard::JV_UNSUPPORTED_COMMAND, "Support later");
}

void AbstractService::Common_SetVersions(XFSIoTCommandEvent *pEvent)
{
    QJsonValue l_jvCommands = pEvent->payLoad()["commands"];
    if (l_jvCommands.isObject()) {
        QJsonObject l_joCommands = l_jvCommands.toObject();
        QStringList l_strLstKeys = l_joCommands.keys();
        for (auto itr = l_strLstKeys.begin(); itr != l_strLstKeys.end(); itr++) {
            QJsonValueRef l_jvVersion = l_joCommands[*itr];
            if (l_jvVersion.isDouble()) {
                int l_iClientID = pEvent->clientId();
                int l_iVersion = l_jvVersion.toInt();
                if (setCommandVersion(l_iClientID, *itr, l_iVersion)) {
                    log(QString("Set version success: Client ID [%1], Command Name [%2], Version [%3]")
                                .arg(l_iClientID) //
                                .arg(*itr) //
                                .arg(l_iVersion));
                } else {
                    notifyCompletion(pEvent, //
                                     XFSIoTStandard::JV_INVALID_DATA, //
                                     QString("Set command [%1] version ERROR").arg(*itr));
                    return;
                }
            } else {
                notifyCompletion(pEvent, //
                                 XFSIoTStandard::JV_INVALID_DATA, //
                                 QString("Version of [%1], not a interger").arg(*itr));
                return;
            }
        }
        notifyCompletion(pEvent, QJsonObject());
    } else {
        notifyCompletion(pEvent, //
                         XFSIoTStandard::JV_INVALID_DATA, //
                         QString("Can't find client to select version"));
    }
}

void AbstractService::Common_Cancel(XFSIoTCommandEvent *pEvent)
{
    log(QString("Cancel: [%1]").arg(pEvent->commandName()));
    const QJsonValue l_jvRequestIDs = pEvent->payLoad()["requestIds"];
    if (l_jvRequestIDs.isUndefined()) {
        log(QString("Cancel all request of Client [%1]").arg(pEvent->clientId()));
        m_pDeviceWorker->cancelCommand(pEvent->clientId());
    } else {
        if (!l_jvRequestIDs.isArray()) {
            notifyCompletion(pEvent, //
                             XFSIoTStandard::JV_INVALID_DATA, //
                             QString("[requestIds] require JSON Array"));
            return;
        } else {
            const QJsonArray l_jaRequestIDs = l_jvRequestIDs.toArray();
            QSet<int> l_iReqestIDs;
            for (auto itr = l_jaRequestIDs.constBegin(); itr != l_jaRequestIDs.constEnd(); itr++) {
                if ((*itr).isDouble()) {
                    l_iReqestIDs.insert((*itr).toInt());
                } else {
                    notifyCompletion(pEvent, //
                                     XFSIoTStandard::JV_INVALID_DATA, //
                                     QString("[requestIds][x] require JSON number"));
                    return;
                }
            }
            m_pDeviceWorker->cancelCommand(pEvent->clientId(), l_iReqestIDs);
        }
    }
    notifyCompletion(pEvent, QJsonObject());
}

bool AbstractService::load(const QJsonArray &jaInterfacesList)
{
    for (auto it = jaInterfacesList.begin(); it != jaInterfacesList.end(); it++) {
        if (it->isString()) {
            XFSIoTInterface *l_pInterface = s_pInterfaceRepository->getInterface(it->toString());
            if (l_pInterface == nullptr) {
                error(QString("Unknow interface [%1]").arg(it->toString()));
                return false;
            } else {
                addInterface(l_pInterface);
            }
        } else {
            error(QString("Config service error, value of interface name not a string, it is [%1]")
                          .arg(JSonUtils::typeOfValue(*it)));
            return false;
        }
    }
    return true;
}

bool AbstractService::init()
{
    log("Start worker");
    if (m_pDeviceWorker != nullptr) {
        m_pDeviceWorker->run();
    } else {
        warn("Worker is NULLL");
    }
    log("Check supported command");
    for (auto itr = m_hInterfacesList.constBegin(); itr != m_hInterfacesList.constEnd(); itr++) {
        log(QString("Check interface [%1]").arg((*itr)->name()));
        auto l_functionList = (*itr)->functionsList();
        for (auto itrFunctioName = l_functionList.constBegin(); itrFunctioName != l_functionList.constEnd();
             itrFunctioName++) {
            if ((*itrFunctioName)->isSupport(this)) {
                log(QString("Function [%1] : supported").arg(itrFunctioName.key()));
            } else {
                warn(QString("Function [%1] : UNSUPPORTED").arg(itrFunctioName.key()));
            }
        }
    }
    return true;
}

void AbstractService::addInterface(XFSIoTInterface *pInterface)
{
    if (m_hInterfacesList.contains(pInterface->name())) {
        delete m_hInterfacesList.take(pInterface->name());
    }
    m_hInterfacesList.insert(pInterface->name(), pInterface);
}

void AbstractService::notifyEvent(XFSIoTMsgEvent *event) const
{
    log(QString("Notify EVENT [%1] to Endpoint").arg(event->xfsIotType()));
    QCoreApplication::postEvent(m_pEndpoint, event);
}

bool AbstractService::setCommandVersion(int iClientID, const QString &strCommandName, int iVersion)
{
    if (m_hhVersionSelected.contains(iClientID)) {
        m_hhVersionSelected[iClientID].insert(strCommandName, iVersion);
        return true;
    } else {
        error(QString("Can't find client id [%1]").arg(iClientID));
        return false;
    }
}

void AbstractService::notifyCompletion(const XFSIoTCommandEvent *pEventCmd, const QJsonObject &joPayload) const
{
    notifyEvent(new XFSIoTCompletionEvent(pEventCmd, joPayload));
}

void AbstractService::notifyCanceled(const XFSIoTCommandEvent *pEventCmd) const
{
    notifyCompletion(pEventCmd, XFSIoTStandard::JV_CANCELED);
}

void AbstractService::notifyTimeOut(const XFSIoTCommandEvent *pEventCmd) const
{
    notifyCompletion(pEventCmd, XFSIoTStandard::JV_COMPLETION_CODE_timeOut);
}

void AbstractService::notifyEventEvent(const XFSIoTCommandEvent *pEventCmd, //
                                       const QString &strEventName, //
                                       const QJsonObject &joPayload) const
{
    notifyEvent(new XFSIoTEventEvent(pEventCmd, strEventName, joPayload));
}

bool AbstractService::updateStatus()
{
    m_joStatusCommon["lastUpdate"] = QDateTime::currentDateTime().toString();
    return true;
}

void AbstractService::queueCommand(const XFSIoTCommandEvent *pEventCmd) const
{
    DeviceWorker *l_pWorker = qobject_cast<DeviceWorker *>(m_pDeviceWorker);
    if (l_pWorker != nullptr) {
        l_pWorker->queueCommand(pEventCmd);
    } else {
        error("Device Worker not XFSIoT Worker");
    }
}

bool AbstractService::isSupportInterface(const QString &strInterfaceName) const
{
    return m_hInterfacesList.contains(strInterfaceName);
}

void AbstractService::notifyCompletion(const XFSIoTCommandEvent *pEventCmd, //
                                       const QString &strCompletionCode, //
                                       const QString &strErrorDescription) const
{
    QJsonObject l_joPayload;
    XFSIoTStandard::buildPayloadCompletion(l_joPayload, //
                                           strCompletionCode, //
                                           strErrorDescription);
    if (!strErrorDescription.isEmpty()) {
        error(strErrorDescription);
    }
    notifyEvent(new XFSIoTCompletionEvent(pEventCmd, l_joPayload));
}

void AbstractService::notifyCompletionErrorCode(const XFSIoTCommandEvent *pEventCmd, //
                                                const QString &strErrorCode, //
                                                const QString &strErrorDescription) const
{
    QJsonObject l_joPayload;
    l_joPayload["errorCode"] = strErrorCode;
    notifyCompletion(pEventCmd, XFSIoTStandard::JV_COMMAND_ERROR_CODE, strErrorDescription);
}

void AbstractService::notifyInvalidCommand(const XFSIoTCommandEvent *pEventCmd,
                                           const QString &strErrorDescription) const
{
    notifyCompletion(pEventCmd, XFSIoTStandard::JV_INVALID_COMMAND, strErrorDescription);
}

bool AbstractService::onCommand(XFSIoTCommandEvent *pEvent)
{
    debug(QString("onCommand [%1]").arg(pEvent->commandName()));

    QStringList l_strCmdStruct = pEvent->commandName().split('.');
    if (l_strCmdStruct.size() != 2) {
        notifyInvalidCommand(pEvent, //
                             QString("Invalid command name, it have to [pattern: ^[0-9A-Za-z]*\\.[0-9A-Za-z]*$]") //
                                     .arg(pEvent->commandName()));
    } else {
        debug(QString("Call: Client [%1] -> [%2].[%3]") //
                      .arg(pEvent->clientId()) //
                      .arg(l_strCmdStruct[0], l_strCmdStruct[1]));
        if (this->isSupportInterface(l_strCmdStruct[0])) {
            XFSIoTInterface *l_pInterface = m_hInterfacesList.value(l_strCmdStruct[0]);
            bool l_isSuccess = l_pInterface->invokeServiceFunction(l_strCmdStruct[1], this, pEvent);
            if (l_isSuccess) {
                debug(QString("Call [%1->%2] success").arg(l_strCmdStruct[0], l_strCmdStruct[1]));
            } else {
                notifyInvalidCommand(pEvent, QString("Service not support fuction [%1]").arg(pEvent->commandName()));
            }
        } else {
            notifyInvalidCommand(pEvent, QString("Not support interface [%1]").arg(l_strCmdStruct[0]));
        }
    }
    return true;
}

bool AbstractService::onClientConnected(int iClientID)
{
    log(QString("Client [%1] connected").arg(iClientID));
    m_hhVersionSelected.insert(iClientID, QHash<QString, int>());
    return true;
}

bool AbstractService::onClientDisconnected(int iClientID)
{
    log(QString("Client [%1] Disconnected").arg(iClientID));
    m_hhVersionSelected.remove(iClientID);
    return true;
}

bool AbstractService::event(QEvent *event)
{
    if (event->type() == XFSIoTMsgEvent::ClientConnected) {
        onClientConnected(((XFSIoTClientEvent *)event)->clientID());
    } else if (event->type() == XFSIoTMsgEvent::ClientDisconnect) {
        onClientDisconnected(((XFSIoTClientEvent *)event)->clientID());
    } else if (event->type() == XFSIoTMsgEvent::CommandEvent) {
        onCommand((XFSIoTCommandEvent *)event);
    }
    return SelfServiceObject::event(event);
}

bool AbstractService::loadConfig(const QJsonObject &config)
{
    if (SelfServiceObject::loadConfig(config)) {
        const QJsonValue l_jvPayloadItems = config["payloadItems"];
        if (l_jvPayloadItems.isObject()) {
            m_joPayloadItems = l_jvPayloadItems.toObject();
            m_joStatusCommon = m_joPayloadItems["Status.common"].toObject();
        } else {
            error("payloadDefault require a Objects");
            return false;
        }
        m_pDeviceWorker = loadDeviceWorker(config["deviceWorker"]);
        if (m_pDeviceWorker == nullptr) {
            error("Load device worker ERROR");
            return false;
        }
    } else {
        return false;
    }
    return true;
}

const QJsonValue AbstractService::payloadItem(const QString &strName) const
{
    return m_joPayloadItems[strName];
}
