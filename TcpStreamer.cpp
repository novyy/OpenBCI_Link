// Name: TcpStreamer.cpp
// Date: 04-Nov-2020
// Purpose: Streams Cyton data to tcp server
// Author: Piotr Nowinski

#include "TcpStreamer.h"

TcpStreamer::~TcpStreamer()
{
    if(m_pClient != nullptr) m_pClient->stop();
}

Result TcpStreamer::connect(const Endpoint& endpoint)
{
    if(m_pClient != nullptr) m_pClient->stop();

    auto pClient = std::unique_ptr<WiFiClient>(new WiFiClient());
    if(pClient->connect(endpoint.ipAddress, endpoint.port))
    {
        pClient->setNoDelay(true);
        m_remoteEndpoint = endpoint;
        m_pClient = std::move(pClient);
        return Result();
    }
    else return Result(false, "Failed to connect to server");
}

void TcpStreamer::disconnect()
{
  if(m_pClient == nullptr) return;
  m_pClient->stop();
  m_pClient.reset();
}

Result TcpStreamer::send(const uint8_t* pData, size_t size)
{
    if((m_pClient == nullptr) || (m_pClient->connected() == false)) return Result(false, "TCP client not initilaized");

    const bool result = m_pClient->write(pData, size);
    if(result != size) return Result(false, String("TCP client doesn't write enought bytes. ") + String(result) + " out of " + String(size));
    else Result();
}

const Endpoint& TcpStreamer::getRemoteEndpoint() const
{
  return m_remoteEndpoint;
}

bool TcpStreamer::connected()
{
  return (m_pClient != nullptr) && (m_pClient->connected());
}
