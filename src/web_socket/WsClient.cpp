#include "WsClient.h"
#include "Log.h"


WebSocketClient::WebSocketClient(const std::string& url)
{
    using easywsclient::WebSocket;

    // Websocket thread that sends and receives messages from and to the server and stores them in the class queues
    m_Runner = std::thread([=] {
        std::unique_ptr<WebSocket> ws(WebSocket::from_url(url));
        
        if (!ws)    // Check if the connection was successful
        {
            LOG_ERROR("Failed to connect websocket to url: {}", url);
            return;
        }

        // Main send and receive loop
        while (m_Running) {
            if (ws->getReadyState() == WebSocket::CLOSED)
                break;
            std::string data;
            if (m_Outgoing.pop(data))
                ws->send(data);
            ws->poll();
            ws->dispatch([&](const std::string& message) {
                m_Incoming.push(message);
            });
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        ws->close();
        ws->poll();
    });
}

WebSocketClient::~WebSocketClient()
{ 
    m_Running = false;
    if (m_Runner.joinable())    m_Runner.join(); 
}
