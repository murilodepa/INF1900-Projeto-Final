# CMakeLists.txt in the src directory

cmake_minimum_required(VERSION 3.14)

# C++ version ===========================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Source files
set(SRC_FILES
    # Main
    "TrucoGame.cpp"

    # Controllers
    "controllers/Application.cpp"
    "controllers/TableController.cpp"
    "controllers/GMController.cpp"

    # Models
    "models/client/ClientGameManager.cpp"
    "models/client/TcpClient.cpp"
    "models/packets/Packet.cpp"
    "models/server/AIPlayer.cpp"
    "models/server/ServerGameManager.cpp"
    "models/server/TcpClientPlayer.cpp"
    "models/server/TcpServer.cpp"
    "models/Card.cpp"
    "models/Deck.cpp"
    "models/Table.cpp"
    "models/Player.cpp"
    "models/Score.cpp"
    
    # Views
    "views/button/ButtonBase.cpp"
    "views/mouse/MouseState.cpp"
    "views/text/TextView.cpp"
    "views/trucoGameView/buttonsTruco/AcceptTrucoButton.cpp"
    "views/trucoGameView/buttonsTruco/CardButton.cpp"
    "views/trucoGameView/buttonsTruco/IncreaseStakesButton.cpp"
    "views/trucoGameView/buttonsTruco/RefuseTrucoButton.cpp"
    "views/trucoGameView/buttonsTruco/TrucoButton.cpp"
    "views/trucoGameView/player/PlayerCards.cpp"
    "views/trucoGameView/player/PlayerView.cpp"
    "views/trucoGameView/Animator.cpp"
    "views/trucoGameView/CardView.cpp"
    "views/trucoGameView/PopUpTruco.cpp"
    "views/trucoGameView/ScoreView.cpp"
    "views/trucoGameView/TableView.cpp"
    "views/trucoGameView/TrucoGameView.cpp"
    "views/GraphicManager.cpp"
    "views/utils/MutexView.cpp"
    "views/utils/StatesView.cpp"
    "views/utils/UtilsView.cpp"
    "views/utils/StartInputWindow.cpp"
)

# Add source to this project's executable.
add_executable (TrucoGame ${SRC_FILES})

target_link_libraries(TrucoGame 
sfml-graphics 
nlohmann_json
)

install(TARGETS TrucoGame 
ARCHIVE DESTINATION "TrucoGame/lib"
RUNTIME DESTINATION "TrucoGame/lib")
install(DIRECTORY "include"
        DESTINATION "TrucoGame"
)