#include <Nephilim/Network/SocketSelector.hpp>
#include <Nephilim/Network/Socket.h>
#include "SocketImpl.hpp"

#include <utility>

#ifdef _MSC_VER
    #pragma warning(disable : 4127) // "conditional expression is constant" generated by the FD_SET macro
#endif

NEPHILIM_NS_BEGIN

////////////////////////////////////////////////////////////
struct SocketSelector::SocketSelectorImpl
{
    fd_set AllSockets;   ///< Set containing all the sockets handles
    fd_set SocketsReady; ///< Set containing handles of the sockets that are ready
    int    MaxSocket;    ///< Maximum socket handle
};


////////////////////////////////////////////////////////////
SocketSelector::SocketSelector() :
m_impl(new SocketSelectorImpl)
{
    clear();
}


////////////////////////////////////////////////////////////
SocketSelector::SocketSelector(const SocketSelector& copy) :
m_impl(new SocketSelectorImpl(*copy.m_impl))
{

}


////////////////////////////////////////////////////////////
SocketSelector::~SocketSelector()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
void SocketSelector::add(Socket& socket)
{
    SocketHandle handle = socket.getHandle();
    if (handle != priv::SocketImpl::invalidSocket())
    {
        FD_SET(handle, &m_impl->AllSockets);

        int size = static_cast<int>(handle);
        if (size > m_impl->MaxSocket)
            m_impl->MaxSocket = size;
    }
}


////////////////////////////////////////////////////////////
void SocketSelector::remove(Socket& socket)
{
    FD_CLR(socket.getHandle(), &m_impl->AllSockets);
    FD_CLR(socket.getHandle(), &m_impl->SocketsReady);
}


////////////////////////////////////////////////////////////
void SocketSelector::clear()
{
    FD_ZERO(&m_impl->AllSockets);
    FD_ZERO(&m_impl->SocketsReady);

    m_impl->MaxSocket = 0;
}


////////////////////////////////////////////////////////////
bool SocketSelector::wait(Time timeout)
{
    // Setup the timeout
    timeval time;
    time.tv_sec  = static_cast<long>(timeout.microseconds() / 1000000);
    time.tv_usec = static_cast<long>(timeout.microseconds() % 1000000);

    // Initialize the set that will contain the sockets that are ready
    m_impl->SocketsReady = m_impl->AllSockets;

    // Wait until one of the sockets is ready for reading, or timeout is reached
    int count = select(m_impl->MaxSocket + 1, &m_impl->SocketsReady, NULL, NULL, timeout != Time::Zero ? &time : NULL);

    return count > 0;
}


////////////////////////////////////////////////////////////
bool SocketSelector::isReady(Socket& socket) const
{
    return FD_ISSET(socket.getHandle(), &m_impl->SocketsReady) != 0;
}


////////////////////////////////////////////////////////////
SocketSelector& SocketSelector::operator =(const SocketSelector& right)
{
    SocketSelector temp(right);

    std::swap(m_impl, temp.m_impl);

    return *this;
}

NEPHILIM_NS_END