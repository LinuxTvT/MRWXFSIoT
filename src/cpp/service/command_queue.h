#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "event/xfs_iot_msg_event.h"
#include "qmutex.h"
#include <QSemaphore>

class CommandQueue
{
public:
    CommandQueue();

    bool pushCommand(XFSIoTCommandEvent *pCommandEvent);
    bool cancelCommand(uint uiClientID, int iRequestID = -1);
    // bool cancelCommand(uint uiClientID, const QSet<int> &iSetRequestIds);
    XFSIoTCommandEvent *executeCommand();
    void completedExecute();

private:
    XFSIoTCommandEvent *m_pCommandExecuting = nullptr;
    QList<XFSIoTCommandEvent *> m_listCommands;
    QSemaphore m_semCommands;
    QMutex m_mutex;
};

#endif // COMMANDQUEUE_H
