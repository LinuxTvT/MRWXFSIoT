#include "command_queue.h"

CommandQueue::CommandQueue() { }

bool CommandQueue::pushCommand(XFSIoTCommandEvent *pCommandEvent)
{
    m_mutex.lock();
    m_listCommands.append(pCommandEvent);
    m_semCommands.release();
    m_mutex.unlock();
    return true;
}

bool CommandQueue::cancelCommand(uint uiClientID, int iRequestID)
{
    // Checking in queue
    m_mutex.lock();
    for (auto itr = m_listCommands.begin(); itr != m_listCommands.end(); itr++) {
        if ((*itr)->is(uiClientID, iRequestID)) {
            (*itr)->cancel();
        }
    }
    if (m_pCommandExecuting != nullptr && m_pCommandExecuting->is(uiClientID, iRequestID)) {
        m_pCommandExecuting->cancel();
    }
    m_mutex.unlock();
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

XFSIoTCommandEvent *CommandQueue::executeCommand()
{
    m_semCommands.acquire();
    m_mutex.lock();
    m_pCommandExecuting = m_listCommands.takeFirst();
    m_mutex.unlock();
    return m_pCommandExecuting;
}

void CommandQueue::completedExecute()
{
    m_mutex.lock();
    delete m_pCommandExecuting;
    m_pCommandExecuting = nullptr;
    m_mutex.unlock();
}
