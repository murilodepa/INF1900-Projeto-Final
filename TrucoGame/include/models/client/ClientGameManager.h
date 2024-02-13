#pragma once
#include "TcpClient.h";
#include "../Player.h";
#include "../packets/StartGamePacket.h"
#include "../packets/StartRoundPacket.h"
#include "../packets/PlayerPlayPacket.h"
#include "../packets/CardPacket.h"
#include "../packets/TrucoPacket.h"
#include "../packets/ElevenHandPacket.h"
#include "../packets/ElevenHandResponsePacket.h"

namespace TrucoGame {
    namespace Models {
        class ClientGameManager {
        private:
            Player* player;
            TcpClient client;
            void GetPlayerInputAndSend();
            void OnStartGamePacketReceived(StartGamePacket packet);
            void OnStartRoundPacketReceived(StartRoundPacket packet);
            void OnPlayPacketReceived(PlayerPlayPacket packet);
            void OnTrucoPacketReceived(TrucoPacket packet);
            void OnElevenHandPacketReceived(ElevenHandPacket packet);
        public:
            ClientGameManager();
            void Start(std::string ip);
        };
    }
}