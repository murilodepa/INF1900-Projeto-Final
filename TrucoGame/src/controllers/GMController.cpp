#include "..\..\include\controllers\GMController.h"
#include "../../include/views/utils/StatesView.h"
#include "../../include/views/utils/MutexView.h"

namespace TrucoGame {
	namespace Controller {

		// ---------------------------
		// MODEL -> CONTROLLER -> VIEW
		// ---------------------------

		GMController::GMController(TrucoGameView* gameView, ClientGameManager* gameModel) :
			gameView(gameView),
			gameModel(gameModel)
		{
			gameModel->myTurnStarted = [this](bool canRequestTruco) { OnMyTurnStarted(canRequestTruco); };
			gameModel->anotherPlayerPlayed = [this](Card card, int playerId, bool isCovered) { OnAnotherPlayerPlayed(card, playerId, isCovered); };
			gameModel->roundStarted = [this](Card tableCard, std::vector<Card> handCards) { OnRoundStarted(tableCard, handCards); };
			gameModel->elevenHandRoundStarted = [this](Card tableCard, std::vector<Card> handCards, std::vector<Card> partnerHandCards) { OnElevenHandRoundStart(tableCard, handCards, partnerHandCards); };
			gameModel->ironHandRoundStarted = [this](Card tableCard) { OnIronHandRoundStarted(tableCard); };
			gameModel->roundEnded = [this](int winnerTeamId, int team0Score, int team1Score) { OnRoundEnded(winnerTeamId, team0Score, team1Score); };
			gameModel->turnEnded = [this](int winnerTeamId, int winnerPlayerId) { OnTurnEnded(winnerTeamId, winnerPlayerId); };
			gameModel->trucoAccepted = [this](int currentStakes) { OnTrucoAccepted(currentStakes); };
			gameModel->trucoRefused = [this]() { OnTrucoRefused(); };
			gameModel->trucoRequested = [this](int requesterId, int currentStakes) { OnTrucoResquested(requesterId, currentStakes); };
			gameModel->gameWon = [this]() { OnGameWon(); };
			gameModel->gameLost = [this]() { OnGameLost(); };

			gameView->userSelectCard = [this](int index, bool covered) { UserSelectedCard(index, covered); };
		}

		void GMController::OnMyTurnStarted(bool canRequestTruco){
			// TODO: enable card buttons and truco button if needed

			isPlayerTurnToPlayMutex.lock();
			isPlayerTurnToPlayState = IsPlayerTurnToPlayState::PlayerTurn;
			isPlayerTurnToPlayMutex.unlock();
		}
		void GMController::OnAnotherPlayerPlayed(Card card, int playerId, bool isCovered) {
			int viewPlayerId = ModelIdToViewId(playerId);

			cardStructMutex.lock();
			cardStructState.rank = CardRank(card.getNumber());
			cardStructState.suit = CardSuit(card.getSuit());
			cardStructState.index = CardIndex(currentTurn);
			cardStructMutex.unlock();

			playerIdMutex.lock();
			playerIdState = PlayerIdState(viewPlayerId);
			playerIdMutex.unlock();

			cardMutex.lock();
			if (isCovered) {
				cardState = CardState::Covered;
			}
			else {
				cardState = CardState::TurnedUp;
			}

			cardMutex.unlock();

			discardCardMutex.lock();
			discardCardState = DiscardCardState::DiscardCard;
			discardCardMutex.unlock();
			
			
			//std::string texturePathToturnedFaceUpCard = UtilsView::findTexturePathByNumberAndSuit(cardStruct);
			// TODO: show other card player on the screen
		}

		void GMController::OnRoundStarted(Card tableCard, std::vector<Card> handCards){
			// TODO: show hand cards and table card (keep card buttons disabled)
			
			CardStruct cardStruct;

			cardStruct.rank = CardRank(tableCard.getNumber());
			cardStruct.suit = CardSuit(tableCard.getSuit());
			std::string texturePathToturnedFaceUpCard = UtilsView::findTexturePathByNumberAndSuit(cardStruct);
			gameView->setTexturePathToturnedFaceUpCard(texturePathToturnedFaceUpCard);

			std::vector<std::string> texturePathToMainPlayerCards;
			for (Card& card : handCards)
			{
				cardStruct.rank = CardRank(card.getNumber());
				cardStruct.suit = CardSuit(card.getSuit());
				texturePathToMainPlayerCards.emplace_back(UtilsView::findTexturePathByNumberAndSuit(cardStruct));
			}

			gameView->setTexturePathToMainPlayerCards(texturePathToMainPlayerCards);

			distributeCardsToPlayersMutex.lock();
			distributeCardsToPlayersState = DistributeCardsToPlayersState::Distribute;
			distributeCardsToPlayersMutex.unlock();
		}
		void GMController::OnElevenHandRoundStart(Card tableCard, std::vector<Card> handCards, std::vector<Card> partnerHandCards){
			// TODO: show partner cards, show hand cards (keep card buttons disabled)
			// and show [yes, no] menu: where the response should somehow call for UserRespondedElevenHand

		}
		void GMController::OnIronHandRoundStarted(Card tableCard){
			// TODO: just show table card (keep card buttons disabled)
		}

		void GMController::OnRoundEnded(int winnerTeamId, int team0Score, int team1Score){
			this->currentTurn = 0;
			// TODO: update score on the screen, clear cards from hands
		}

		void GMController::OnTurnEnded(int winnerTeamId, int winnerPlayerId){
			this->currentTurn++;
			// TODO: update score on the screen (the circles) and clear cards from table to the next turn
		}

		void GMController::OnTrucoAccepted(int currentStakes){
			// TODO: update stakes on the screen
		}
		void GMController::OnTrucoRefused(){
			// TODO
		}
		void GMController::OnTrucoResquested(int requesterId, int currentStakes){
			// TODO: show popup on the screen to responde [yes = 0, no = 1, raise = 2]
			// where the response should somehow call for UserRespondedTruco
		}

		void GMController::OnGameWon(){
			// TODO
		}
		void GMController::OnGameLost(){
			// TODO
		}

		// ---------------------------
		// VIEW -> CONTROLLER -> MODEL
		// ---------------------------

		void GMController::UserRequestedTruco() {
			gameModel->RequestTruco();
		}
		void GMController::UserSelectedCard(int cardIndex, bool isCovered) {
			// TODO: disable card button
			gameModel->PlayCard(cardIndex, isCovered);
		}
		void GMController::UserRespondedTruco(int trucoResult) {
			gameModel->RespondTrucoRequest(trucoResult);
		}
		void GMController::UserRespondedElevenHand(bool accepted) {
			gameModel->RespondElevenHand(accepted);
		}


		int GMController::ModelIdToViewId(int modelId)
		{
			int myId = gameModel->player->playerId;
			return ((modelId + 2) - myId) % 4;
		}

		bool GMController::IsMyTeam(int teamId) {
			return (gameModel->player->playerId % 2) == teamId;
		}
	}
}

