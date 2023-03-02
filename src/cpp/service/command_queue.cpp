#include "command_queue.h"

CommandQueue::CommandQueue() { }

bool CommandQueue::enqueueCommand(XFSIoTCommandEvent *pCommandEvent)
{

    QMutexLocker LOCKER(&m_mutex);
    m_listCommands.append(pCommandEvent);
    m_semCommands.release();

    return true;
}

bool CommandQueue::cancelCommand(uint uiClientID, int iRequestID)
{
    QMutexLocker LOCKER(&m_mutex);
    for (auto itr = m_listCommands.begin(); itr != m_listCommands.end(); itr++) {
        if ((*itr)->is(uiClientID, iRequestID)) {
            (*itr)->cancel();
        }
    }
    if (m_pCommandExecuting != nullptr && m_pCommandExecuting->is(uiClientID, iRequestID)) {
        m_pCommandExecuting->cancel();
    }

    return true;
}

// bool CommandQueue::cancelCommand(uint uiClientID, const QSet<int> &iSetRequestIds)
//{
//    // Checking in queue
//    m_mutex.lock();
//    for (auto itr = m_listCommands.begin(); itr != m_listCommands.end(); itr++) {
//        if ((*itr)->is(uiClientID, iSetRequestIds)) {
//            (*itr)->cancel();
//        }
//    }
//    if (m_pCommandExecuting != nullptr && m_pCommandExecuting->is(uiClientID, iSetRequestIds)) {
//        m_pCommandExecuting->cancel();
//    }
//    m_mutex.unlock();
//    return true;
//}

XFSIoTCommandEvent *CommandQueue::dequeueCommand(int iWaitTimeout)
{
    if (m_semCommands.tryAcquire(1, iWaitTimeout)) {
        QMutexLocker LOCKER(&m_mutex);
        m_pCommandExecuting = m_listCommands.takeFirst();
        return m_pCommandExecuting;
    } else {
        return nullptr;
    }
}

void CommandQueue::completedExecute()
{
    QMutexLocker LOCKER(&m_mutex);
    delete m_pCommandExecuting;
    m_pCommandExecuting = nullptr;
}
