﻿// TrucoGame.cpp : Defines the entry point for the application.

#include <SFML/Graphics.hpp>
#include "../include/controllers/Application.h"

#pragma region TestTCP

#include <thread>
#include "../include/models/server/TcpServer.h"
#include "../include/models/client/TcpClient.h"
#include "../include/models/Card.h"
#include "../include/models/packets/Packet.h"
#include "../include/models/packets/StartGamePacket.h"
#include "../include/models/packets/CardPacket.h"
#include "../include/models/packets/EndRoundPacket.h"
#include "../include/models/packets/PlayerPlayPacket.h"
#include "../include/models/packets/EndTurnPacket.h"
#include "../include/models/packets/TrucoPacket.h"

#define TEST_CLIENT
void Client();
void Server();

void Server() {
    using namespace TrucoGame::Models;
    std::cout << "[SERVER] Starting Server Thread" << std::endl;
    TrucoGame::Models::TcpServer server;
    server.Open(12345);

    std::thread clientThread(Client);

    server.StartAcceptingClients();
    server.StopAcceptingClients();
    server.StartListeningClients();

#ifdef TEST_CLIENT
    int choice;
    while (true) {
        std::cout << "************************" << std::endl;
        std::cout << "[SERVER] 0 - StartGame" << std::endl;
        std::cout << "[SERVER] 1 - StartRoundPacket" << std::endl;
        std::cout << "[SERVER] 2 - EndRoundPacket" << std::endl;
        std::cout << "[SERVER] 3 - EndTurnPacket" << std::endl;
        std::cout << "[SERVER] 4 - PlayerPlayPacket" << std::endl;
        std::cout << "[SERVER] 5 - PlayerCard" << std::endl;
        std::cout << "[SERVER] 6 - TrucoPacket" << std::endl;
        std::cout << "************************" << std::endl;

        std::cin >> choice;
        switch (choice)
        {
        case 0:
        {
            StartGamePacket startGame(1, 1);
            server.SendToAllClients(&startGame);
            break;
        }
        case 1:
        {
            ClubsCard tableCard(1);
            std::vector<TrucoGame::Models::Card> handCards;
            handCards.push_back(ClubsCard(2));
            handCards.push_back(HeartsCard(1));
            handCards.push_back(SpadesCard(3));

            StartRoundPacket startRound(tableCard, handCards);

            std::cout << "[SERVER] Sending 3 of Clubs Card\n";
            server.SendToAllClients(&startRound);
            break;
        }
        case 2:
        {
            EndRoundPacket endRound(1, 3, 0, 3);
            server.SendToAllClients(&endRound);
            break;
        }
        case 3:
        {
            EndTurnPacket endTurn(1, 3);
            server.SendToAllClients(&endTurn);
            break;
        }
        case 4:
        {
            PlayerPlayPacket playerPlay(1);
            server.SendToAllClients(&playerPlay);
            break;
        }
        case 5:
        {
            ClubsCard card(3);
            CardPacket cardPacket(2, card);
            std::cout << "[SERVER] Sending 3 of Clubs Card\n";
            server.SendToAllClients(&cardPacket);
            break;
        }
        case 6:
        {
            TrucoPacket trucoPacket(1, 2, TrucoResult::Yes);
            server.SendToAllClients(&trucoPacket);
            break;
        }
        default:
            break;
        }
    }
#else
    while (true) {}
#endif 

    clientThread.join();
}

#include <chrono>
#include <thread>

void OnPacketReceived(TrucoGame::Models::Packet packet) {
    //
}

void Client() {
    std::cout << "[CLIENT] Starting client Thread" << std::endl;

    TrucoGame::Models::TcpClient client;
    ErrorCode result = ErrorCode::SocketError;

    while (HAS_FAILED(result)) {
        result = client.Connect("127.0.0.1", 12345);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    client.StartListening();

#ifdef TEST_SERVER
    using namespace TrucoGame::Models;
    int choice;
    while (true) {
        std::cout << "************************" << std::endl;
        std::cout << "[CLIENT] 5 - PlayerCard" << std::endl;
        std::cout << "[CLIENT] 6 - TrucoPacket" << std::endl;
        std::cout << "************************" << std::endl;

        std::cin >> choice;
        switch (choice)
        {
        case 5:
        {
            ClubsCard card(3);
            CardPacket cardPacket(2, card);
            std::cout << "[SERVER] Sending 3 of Clubs Card\n";
            client.Send(&cardPacket);
            break;
        }
        case 6:
        {
            TrucoPacket trucoPacket(1, 2, TrucoResult::Yes);
            client.Send(&trucoPacket);
            break;
        }
        default:
            break;
        }
    }
#else
    while (true) {}
#endif
}

void TestTcp() {
    int choice;
    std::cout << "1 - SERVER \n2 - CLIENT\n";
    std::cin >> choice;
    if (choice == 1) {
        std::thread tcpThread(Server);
        tcpThread.join();
    }
    else {
        std::thread tcpThread(Client);
        tcpThread.join();
    }
}
#pragma endregion

int main()
{
    TrucoGame::Application applicationObject;
    applicationObject.run();
    TestTcp();
    return 0;
}