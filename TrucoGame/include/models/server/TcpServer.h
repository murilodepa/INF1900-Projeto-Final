#pragma once
#include <winsock2.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "../ErrorCode.h"
#include "../packets/Packet.h"
#include <nlohmann/json.hpp>
#include "TcpClientPlayer.h"

#pragma comment(lib,"WS2_32")
#pragma warning(disable:4996)

namespace TrucoGame {

    namespace Models {

        class TcpServer {
        private:
            SOCKET serverSocket;
            sockaddr_in serverAddr;
            std::thread acceptThread;
            bool running;

        public:
            std::vector<TcpClientPlayer*> players;

            ErrorCode Open(u_short port);
            ErrorCode StartAcceptingClients();
            ErrorCode StopAcceptingClients();

            ErrorCode StartListeningClients();
            ErrorCode SendToClients(std::vector<TcpClientPlayer*> players, Packet* packet);
            std::pair<Packet*, Packet*> TcpServer::WaitForTeamPacket(TcpClientPlayer* players[]);

            std::vector<TcpClientPlayer*> AcceptPlayers(int numberOfClients);


            ErrorCode Close();
            void AcceptClients();

        private:
            ErrorCode InitializeWinSock();
            ErrorCode CreateSocket(SOCKET& socket);
            ErrorCode BindSocket(SOCKET& socket);
        };
    }
}