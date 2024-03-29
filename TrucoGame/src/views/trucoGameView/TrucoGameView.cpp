#include "../../../include/views/trucoGameView/TrucoGameView.h"
#include "../../../include/views/trucoGameView/Animator.h"
#include "../../../include/views/utils/MutexView.h"
#include "../../../include/views/utils/StatesView.h"

#include <thread>
#include <iostream>

void TrucoGame::View::TrucoGameView::initialize(const std::vector<std::string>& playerNames)
{
	for (size_t playerIndex = 0; playerIndex < NUM_PLAYERS; playerIndex++) {
		PlayerView* player = new PlayerView(CARDS_IN_HAND, windowSize.y, playerNames[playerIndex]);
		players.push_back(player);
	}

	cardButtons.resize(CARDS_IN_HAND);
	positionToDiscardCards.resize(NUM_PLAYERS);
	texturePathToMainPlayerCards.resize(CARDS_IN_HAND);
	texturePathToPartnerHandCards.resize(CARDS_IN_HAND);
	//playersCardsOnTable.resize(NUM_PLAYERS);

	if (playerCards.cardsInHands != nullptr) {
		float tableAndCardsSpacing = windowSize.y * CALCULATE_TABLE_AND_CARDS_SPACING;
		float cardsSpacing = windowSize.x * CALCULATE_CARDS_SPACING;
		setCardPositionsOfThePlayers(windowSize.x, windowSize.y, playerCards.cardsInHands[0][0].getCardWidth(), playerCards.cardsInHands[0][0].getCardHeight(), cardsSpacing, tableAndCardsSpacing);
	}

	float textAndTableSpacing = windowSize.y * CALCULATE_TEXT_AND_TABLE_SPACING;
	setNamesPositions(windowSize.x, windowSize.y, textAndTableSpacing);
	notificationsText.setPosition(windowSize.x / 2 - notificationsText.getHalfTextWidth(), windowSize.y / 2 + notificationsText.getTextHeight()*2);
	setPositionToDiscardCards();
	initializeTrucoButton();
}

void TrucoGame::View::TrucoGameView::setCardPositionsOfThePlayers(float screenWidth, float screenHeight, float cardWidth, float cardHeight, float cardsSpacing, float cardAndTableSpacing) {

	float halfScreenWidth = screenWidth / 2;
	float halfScreenHeight = screenHeight / 2;
	float halfCardWidth = cardWidth / 2;

	std::vector<Vector2f> cardsPositions;

	cardsPositions.push_back(Vector2f(halfScreenWidth - halfCardWidth - cardsSpacing - cardWidth, cardAndTableSpacing));
	cardsPositions.push_back(Vector2f(halfScreenWidth - halfCardWidth, cardAndTableSpacing));
	cardsPositions.push_back(Vector2f(halfScreenWidth + halfCardWidth + cardsSpacing, cardAndTableSpacing));
	players[0]->setCardPositions(cardsPositions);

	cardsPositions.clear();

	cardsPositions.push_back(Vector2f(cardHeight + cardAndTableSpacing, halfScreenHeight - halfCardWidth - cardsSpacing - cardWidth));
	cardsPositions.push_back(Vector2f(cardHeight + cardAndTableSpacing, halfScreenHeight - halfCardWidth));
	cardsPositions.push_back(Vector2f(cardHeight + cardAndTableSpacing, halfScreenHeight + halfCardWidth + cardsSpacing));
	players[1]->setCardPositions(cardsPositions);


	cardsPositions.clear();

	cardsPositions.push_back(Vector2f(halfScreenWidth - halfCardWidth - cardsSpacing - cardWidth + cardWidth, screenHeight - cardAndTableSpacing - cardHeight));
	cardsPositions.push_back(Vector2f(halfScreenWidth - halfCardWidth + cardWidth, screenHeight - cardAndTableSpacing - cardHeight));
	cardsPositions.push_back(Vector2f(halfScreenWidth + halfCardWidth + cardsSpacing + cardWidth, screenHeight - cardAndTableSpacing - cardHeight));
	players[2]->setCardPositions(cardsPositions);


	cardsPositions.clear();

	cardsPositions.push_back(Vector2f(screenWidth - cardAndTableSpacing, halfScreenHeight - halfCardWidth - cardsSpacing - cardWidth));
	cardsPositions.push_back(Vector2f(screenWidth - cardAndTableSpacing, halfScreenHeight - halfCardWidth));
	cardsPositions.push_back(Vector2f(screenWidth - cardAndTableSpacing, halfScreenHeight + halfCardWidth + cardsSpacing));
	players[3]->setCardPositions(cardsPositions);
}

void TrucoGame::View::TrucoGameView::setNamesPositions(float screenWidth, float screenHeight, float textAndTableSpacing)
{
	float halfScreenWidth = screenWidth / 2;
	float halfScreenHeight = screenHeight / 2;
	float halfTextWidth;

	size_t playerIndex = 0;
	PlayerView* player = players[playerIndex];
	halfTextWidth = player->playerName->getHalfTextWidth();
	player->setNamePosition(Vector2f(halfScreenWidth - halfTextWidth, textAndTableSpacing));

	playerIndex = 1;
	player = players[playerIndex];
	halfTextWidth = player->playerName->getHalfTextWidth();
	player->setNamePosition(Vector2f(textAndTableSpacing, halfScreenHeight + halfTextWidth));
	player->setNameRotation(-90.0f);

	playerIndex = 2;
	player = players[playerIndex];
	halfTextWidth = player->playerName->getHalfTextWidth();
	float textHeight = player->playerName->getTextHeight();
	player->setNamePosition(Vector2f(halfScreenWidth - halfTextWidth, screenHeight - textAndTableSpacing - textHeight - 0.3f * textHeight));

	playerIndex = 3;
	player = players[playerIndex];
	halfTextWidth = player->playerName->getHalfTextWidth();
	player->setNamePosition(Vector2f(screenWidth - textAndTableSpacing, halfScreenHeight - halfTextWidth));
	player->setNameRotation(90.0f);
}

void TrucoGame::View::TrucoGameView::setPositionToDiscardCards() {
	//Vector2f(0.70f * windowSize.x, 0.63f * windowSize.y);
	positionToDiscardCards[0] = Vector2f(0.35f * windowSize.x, 0.42f * windowSize.y); // front
	positionToDiscardCards[1] = Vector2f(0.35f * windowSize.x, 0.65f * windowSize.y); // left
	positionToDiscardCards[2] = Vector2f(0.75f * windowSize.x, 0.65f * windowSize.y); //Main player
	positionToDiscardCards[3] = Vector2f(0.75f * windowSize.x, 0.42f * windowSize.y); // right
}

void TrucoGame::View::TrucoGameView::initializeTrucoButton() {

	Vector2f trucoButtonPosition = Vector2f(0.85f * windowSize.x, 0.8f * windowSize.y);
	Vector2f trucoButtonDimensions = Vector2f(0.1f * windowSize.x, 0.06f * windowSize.y);
	trucoButton = new TrucoButton(trucoButtonPosition.x, trucoButtonPosition.y, trucoButtonDimensions.x, trucoButtonDimensions.y);
	trucoButton->setIsButtonAvailable(true);

	trucoButtonPosition = Vector2f(0.1f * windowSize.x, 0.1f * windowSize.y);
	trucoButtonDimensions = Vector2f(0.1f * windowSize.x, 0.1f * windowSize.y);

	//increaseStakesButton = new IncreaseStakesButton(trucoButtonPosition.x, trucoButtonPosition.y, trucoButtonDimensions.x, trucoButtonDimensions.y);

	trucoButtonPosition = Vector2f(0.4f * windowSize.x, 0.4f * windowSize.y);
	trucoButtonDimensions = Vector2f(0.1f * windowSize.x, 0.1f * windowSize.y);
	//	refuseTrucoButton = new RefuseTrucoButton(trucoButtonPosition.x, trucoButtonPosition.y, trucoButtonDimensions.x, trucoButtonDimensions.y);
	trucoButton->trucoButtonClick = [this]() 
	{
	if (trucoEventHandler)
		trucoEventHandler();
	};
}

void TrucoGame::View::TrucoGameView::drawScore(GraphicManager* pGraphicManager)
{
	pGraphicManager->drawElement(scoreView.getScoreRectangle());
	pGraphicManager->drawElement(scoreView.getRectangleDivWidth());
	pGraphicManager->drawElement(scoreView.getRectangleDivHeight());

	for (int i = 0; i < 3; i++) {
		pGraphicManager->drawElement(scoreView.getOurCircles(i));
		pGraphicManager->drawElement(scoreView.getTheirCircles(i));
	}
	roundScoreMutex.lock();
	pGraphicManager->drawElement(scoreView.getRoundScoreText());
	roundScoreMutex.unlock();

	pGraphicManager->drawElement(scoreView.getWeScoreText());
	pGraphicManager->drawElement(scoreView.getTheyScoreText());
	
	pGraphicManager->drawElement(scoreView.getWeText());
	pGraphicManager->drawElement(scoreView.getTheyText());
}

void TrucoGame::View::TrucoGameView::drawCardsOnTheTable(GraphicManager* pGraphicManager)
{
	for (size_t player = 0; player < NUM_PLAYERS; player++) {
		for (size_t card = 0; card < CARDS_IN_HAND; card++) {
			uIThreadMutex.lock();
			pGraphicManager->drawElement(playerCards.cardsInHands[player][card]);
			uIThreadMutex.unlock();
		}
	}
}

void TrucoGame::View::TrucoGameView::drawPlayerNames(GraphicManager* pGraphicManager)
{
	for (size_t playerIndex = 0; playerIndex < NUM_PLAYERS; playerIndex++) {
		pGraphicManager->drawElement(players[playerIndex]->getPlayerName());
	}
}

void TrucoGame::View::TrucoGameView::drawTrucoButton(GraphicManager* pGraphicManager, Vector2f& mousePosView)
{
	if (trucoButton->getIsButtonAvailable()) {
		trucoButton->update(mousePosView);
		pGraphicManager->drawElement(*trucoButton);
		pGraphicManager->drawElement(trucoButton->getText());

		//increaseStakesButton->update(mousePosView);
		//refuseTrucoButton->update(mousePosView);
		//pGraphicManager->drawElement(*increaseStakesButton);
		//pGraphicManager->drawElement(*refuseTrucoButton);
	}
}


void TrucoGame::View::TrucoGameView::checkIftheCardHasBeenDiscardedAndDraw(GraphicManager* pGraphicManager, Vector2f& mousePosView)
{
	for (size_t card = 0; card < CARDS_IN_HAND; card++) {
		CardButton* cardButton = cardButtons[card];
		if (cardButton && cardButton->getAreCardsInTheHandsOfThePlayer()) {
			cardButton->update(mousePosView);
			pGraphicManager->drawElement(*cardButton);
		}
		//if (!cardButtons[0]->getAreCardsInTheHandsOfThePlayer() && !cardButtons[1]->getAreCardsInTheHandsOfThePlayer() && !cardButtons[2]->getAreCardsInTheHandsOfThePlayer()) {
		//	cardButtons[0]->setAreCardsInTheHandsOfThePlayer(true);
		//	testDiscartCards();
		//}
	}
}

void TrucoGame::View::TrucoGameView::distributeCardsToPlayers()
{
	if (playerCards.cardsInHands != nullptr) {
		float tableAndCardsSpacing = windowSize.y * CALCULATE_TABLE_AND_CARDS_SPACING;
		float cardsSpacing = windowSize.x * CALCULATE_CARDS_SPACING;
		setCardPositionsOfThePlayers(windowSize.x, windowSize.y, playerCards.cardsInHands[0][0].getCardWidth(), playerCards.cardsInHands[0][0].getCardHeight(), cardsSpacing, tableAndCardsSpacing);
	}
	std::vector<std::thread*> animationThreads;

	for (size_t player = 0; player < NUM_PLAYERS; player++) {
		for (size_t card = 0; card < CARDS_IN_HAND; card++) {
			CardView* cardView = &playerCards.cardsInHands[player][card];

			float rotation = cardView->getRotation();
			Texture* cardTexture = playerCards.getCardTexture(player, card);

			if (player == 0) {
				trucoRoundMutex.lock();
				if (trucoRoundState != TrucoRoundState::ElevenHandRound) {
					*cardTexture = UtilsView::loadTextureBack();
					animationThreads.push_back(new std::thread(&TrucoGame::View::Animator::moveSpriteTo, 
						std::ref(*cardView), 
						players[player]->getCardPosition(card), 
						animationSpeed));
				}
				else {
					Vector2f destinationPosition = players[player]->getCardPosition(card);
					animationThreads.push_back(new std::thread(&TrucoGame::View::Animator::moveAndFlipCardTurnedFaceUpTo,
						std::ref(*cardView), 
						cardTexture, 
						texturePathToPartnerHandCards[card], 
						Vector2f(destinationPosition.x + cardView->getCardWidth(), destinationPosition.y), 
						animationSpeed, 
						cardScale));
				}
				trucoRoundMutex.unlock();
			}
			else if (player == 2) {
				Vector2f destinationPosition = players[player]->getCardPosition(card);
				trucoRoundMutex.lock();
				if (trucoRoundState != TrucoRoundState::IronHandRound) {
					animationThreads.push_back(new std::thread(&TrucoGame::View::Animator::moveAndFlipCardTurnedFaceUpTo,
						std::ref(*cardView), cardTexture, texturePathToMainPlayerCards[card], destinationPosition, animationSpeed, cardScale));
				}
				else {
					*cardTexture = UtilsView::loadTextureBack();
					animationThreads.push_back(new std::thread(&TrucoGame::View::Animator::moveSpriteTo,
						std::ref(*cardView),
						Vector2f(destinationPosition.x - cardView->getCardWidth(), destinationPosition.y),
						animationSpeed));
				}
				trucoRoundMutex.unlock();

				float width = cardView->getCardWidth();

				cardButtons[card] = new CardButton(destinationPosition.x - width, destinationPosition.y, width, cardView->getCardHeight(), COLOR_CARD_HOVER, cardView, windowSize, animationSpeed, positionToDiscardCards[2], cardTexture, card);
				cardButtons[card]->setAreCardsInTheHandsOfThePlayer(true);
				cardButtons[card]->cardButtonClick = [this](Sprite* card, Vector2f discardOnTheTablePosition, int index, bool covered)
				{
						std::thread* animationThread;

						trucoRoundMutex.lock();
						TrucoRoundState trucoRoundStateLocal = trucoRoundState;
						trucoRoundMutex.unlock();

						if (trucoRoundStateLocal != TrucoRoundState::IronHandRound)
						{
							animationThread = new std::thread(&TrucoGame::View::Animator::animationToDiscardMainPlayerCard,
								std::ref(*card),
								discardOnTheTablePosition,
								90.0f,
								animationSpeed,
								deckPosition
							);
						}
						else {
							Texture* mainPlayerCardTexture = playerCards.getCardTexture(2, index);
							animationThread = new std::thread(&TrucoGame::View::Animator::animationToDiscardCard,
								std::ref(*card),
								playerCards.getCardTexture(2, index),
								texturePathToMainPlayerCards[index],
								Vector2f(discardOnTheTablePosition.x, discardOnTheTablePosition.y + card->getGlobalBounds().width),
								animationSpeed,
								cardScale,
								90.0f,
								deckPosition,
								false
							);
						}

						animationThread->detach();
						delete animationThread;

						playersCardsOnTheTable.emplace_back(card);

						if(userSelectCard)
							userSelectCard(index, covered); 
				};
			}
			else {
				*cardTexture = UtilsView::loadTextureBack();
				animationThreads.push_back(new std::thread(&TrucoGame::View::Animator::protectMoveAndRotateSpriteTo, std::ref(*cardView), players[player]->getCardPosition(card), 90.0f, animationSpeed));
			}
		}
	}

	for (std::thread* t : animationThreads) {
		t->detach();
		delete t;
	}

	roundMutex.lock();
	if (roundState == RoundState::DistribuiteCards) {
		roundState = RoundState::RoundRunning;
	}
	roundMutex.unlock();
}

TrucoGame::View::TrucoGameView::TrucoGameView(const Vector2f windowSize, const float cardScale, Vector2f& initialDeckPosition, const std::vector<std::string>& playerNames) :
	tableView(Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)), initialDeckPosition, cardScale),
	playerCards(NUM_PLAYERS, CARDS_IN_HAND, initialDeckPosition, cardScale),
	scoreView(windowSize),
	notificationsText(TextView("", CALCULATE_SCORE_TEXT_CHARACTER_SIZE, windowSize.x/2, sf::Color::White, Text::Regular))
{
	this->cardScale = cardScale;
	this->animationSpeed = cardScale * CALCULATE_ANIMATION_SPEED;
	this->windowSize = windowSize;
	this->deckPosition = tableView.getDeckPosition();
	this->deckRotation = tableView.getDeckRotation();
	initialize(playerNames);

}


TrucoGame::View::TrucoGameView::~TrucoGameView()
{
	for (PlayerView* player : players) {
		delete player;
	}
}

void TrucoGame::View::TrucoGameView::verifyIfPlayerDiscardedCard()
{
	discardCardMutex.lock();
	if (discardCardState == DiscardCardState::DiscardCard) {
		discardCardState = DiscardCardState::WaitingPlayer;
		discardCard();
	}
	discardCardMutex.unlock();
}

void TrucoGame::View::TrucoGameView::drawElementsOnTheWindow(GraphicManager* pGraphicManager, Vector2f& mousePosView)
{
	if (pGraphicManager) {
		pGraphicManager->drawElement(tableView.getTableCloth());

		distributeCardsToPlayersMutex.lock();
		if (distributeCardsToPlayersState == DistributeCardsToPlayersState::Distribute) {
			distributeCardsToPlayersState = DistributeCardsToPlayersState::NotDistribute;
			distributeCardsToPlayers();
			tableView.moveDeckAndTurnUpCard(cardScale, animationSpeed, texturePathToturnedFaceUpCard);
		}
		distributeCardsToPlayersMutex.unlock();

		drawCardsOnTheTable(pGraphicManager);
		tableView.drawElementsOnTheTable(pGraphicManager);
		drawPlayerNames(pGraphicManager);
		pGraphicManager->drawElement(notificationsText);
		drawScore(pGraphicManager);
		checkIftheCardHasBeenDiscardedAndDraw(pGraphicManager, mousePosView);
		
		isPlayerTurnToPlayMutex.lock();

		checkTrucoRequestMutex.lock();
		CheckTrucoRequestState checkTrucoRequestStateLocal = checkTrucoRequestState;
		checkTrucoRequestMutex.unlock();

		if (isPlayerTurnToPlayState == IsPlayerTurnToPlayState::PlayerTurn && checkTrucoRequestStateLocal == CheckTrucoRequestState::CanTrucoRequest) {
			drawTrucoButton(pGraphicManager, mousePosView);
		}
		isPlayerTurnToPlayMutex.unlock();

		verifyIfPlayerDiscardedCard();
		verifyRoundEnded();
	}
}

void TrucoGame::View::TrucoGameView::setTexturePathToMainPlayerCards(std::vector<std::string>& texturePathToMainPlayerCards)
{
	this->texturePathToMainPlayerCards = texturePathToMainPlayerCards;
}

void TrucoGame::View::TrucoGameView::setTexturePathToPartnerHandCards(std::vector<std::string>& texturePathToPartnerHandCards)
{
	this->texturePathToPartnerHandCards = texturePathToPartnerHandCards;
}

void TrucoGame::View::TrucoGameView::setTexturePathToturnedFaceUpCard(std::string& texturePathToturnedFaceUpCard)
{
	this->texturePathToturnedFaceUpCard = texturePathToturnedFaceUpCard;
}

void TrucoGame::View::TrucoGameView::setCardToDiscard(std::string& texture, bool isCovered, size_t playerIdState, size_t cardIndex)
{
	this->cardToDiscard.isCovered = isCovered;
	this->cardToDiscard.textureToDiscardCard = texture;
	this->cardToDiscard.playerIdState = playerIdState;
	this->cardToDiscard.cardIndex = cardIndex;
}

void TrucoGame::View::TrucoGameView::coverPartnerHandCardsInElevenHandRound()
{
	int partnerIndex = 0;

	for (size_t card = 0; card < CARDS_IN_HAND; card++) {
		CardView* cardView = &playerCards.cardsInHands[partnerIndex][card];
		Texture* cardTexture = playerCards.getCardTexture(partnerIndex, card);
		*cardTexture = UtilsView::loadTextureBack();
	}
}

void TrucoGame::View::TrucoGameView::setCardToDiscard(bool isCovered, size_t playerIdState, size_t cardIndex)
{
	this->cardToDiscard.isCovered = isCovered;
	this->cardToDiscard.textureToDiscardCard = "";
	this->cardToDiscard.playerIdState = playerIdState;
	this->cardToDiscard.cardIndex = cardIndex;
}

void TrucoGame::View::TrucoGameView::discardCard()
{
	size_t player = cardToDiscard.playerIdState;
	size_t cardIndex = cardToDiscard.cardIndex;

	std::string cardTexturePath;
	if (cardToDiscard.isCovered || cardToDiscard.textureToDiscardCard == "") {
		cardTexturePath = CARD_BACK_TEXTURE_PATH;
	}
	else {
		cardTexturePath = cardToDiscard.textureToDiscardCard;
	}
	
	if (player < NUM_PLAYERS && player >= 0 && cardIndex < CARDS_IN_HAND && cardIndex >= 0) {
		CardView* cardView = &playerCards.cardsInHands[player][cardIndex];

		// Blocking wait state
		while (true)
		{
			cardIsBeingDiscarded.lock();
			if (!cardView->getIsCardBeingDiscarded())
			{
				cardIsBeingDiscarded.unlock();
				break;
			}
			cardIsBeingDiscarded.unlock();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		std::thread* animationThread;

		changePlayersCardsOnTheTableMutex.lock();
		playersCardsOnTheTable.emplace_back(cardView);
		changePlayersCardsOnTheTableMutex.unlock();

		if (player == 3) {
			animationThread = new std::thread(&TrucoGame::View::Animator::animationToDiscardCard, 
				std::ref(*cardView), 
				playerCards.getCardTexture(player, cardIndex), 
				cardTexturePath, 
				positionToDiscardCards[3], 
				animationSpeed, 
				cardScale, 
				0, 
				deckPosition,
				cardToDiscard.isCovered
			);
		}  if (player == 0) {
			animationThread = new std::thread(&TrucoGame::View::Animator::animationToDiscardCard, 
				std::ref(*cardView), 
				playerCards.getCardTexture(player, cardIndex), 
				cardTexturePath, 
				positionToDiscardCards[0], 
				animationSpeed, 
				cardScale, 
				90, 
				deckPosition,
				cardToDiscard.isCovered
			);
		}
		else if (player == 1) {
			Vector2f destinationPosition = Vector2f(positionToDiscardCards[1].x, positionToDiscardCards[1].y + cardView->getCardWidth());
			animationThread = new std::thread(&TrucoGame::View::Animator::animationToDiscardCard, 
				std::ref(*cardView), 
				playerCards.getCardTexture(player, cardIndex), 
				cardTexturePath, 
				destinationPosition, 
				animationSpeed, 
				cardScale, 
				0, 
				deckPosition,
				cardToDiscard.isCovered
			);
		}

		if (player != 2 && player != 0 && player != 1) {
			animationThread->detach();
			delete animationThread;
		}
	}
}

void TrucoGame::View::TrucoGameView::verifyRoundEnded()
{
	roundMutex.lock();
	RoundState roundStateLocal = roundState;
	roundMutex.unlock();

	if (roundStateLocal == RoundState::RoundEnded) {
		roundMutex.lock();
		roundState = RoundState::DistribuiteCards;
		roundMutex.unlock();
		std::vector<std::thread*> animationThreads;

		animationThreads.push_back(new std::thread(&TrucoGame::View::Animator::endRoundAndDistribuiteCards,
			playerCards.cardsInHands,
			tableView.getCardTurnedFaceUp(),
			tableView.getDeck()
		));

		for (std::thread* t : animationThreads) {
			t->detach();
			delete t;
		}
	}
}

void TrucoGame::View::TrucoGameView::notifyPlayer(std::string message) {
	try {
		if (notificationThread != nullptr)
			notificationThread->join();
		notificationThread = new std::thread([this](std::string message) {
			notificationsText.setText(message);
			notificationsText.setPosition(windowSize.x / 2 - notificationsText.getHalfTextWidth(), windowSize.y / 2 + notificationsText.getTextHeight() * 2);
			std::chrono::seconds sleepDuration(2);
			std::this_thread::sleep_for(sleepDuration);
			notificationsText.setText("");
			}, message);
	}
	catch (const std::exception& e) {
		// Handle the exception here, such as logging or displaying an error message.
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	catch (...) {
		// Catch all other exceptions (not recommended, but can be used for generic handling)
		std::cerr << "Unknown exception caught" << std::endl;
	}
}