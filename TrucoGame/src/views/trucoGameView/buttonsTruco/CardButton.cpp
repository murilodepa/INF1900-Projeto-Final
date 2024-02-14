#include "../../../../include/views/trucoGameView/buttonsTruco/CardButton.h"
#include "../../../../include/views/trucoGameView/Animator.h"

#include <thread>
#include "../../../../include/views/utils/MutexView.h"
#include "../../../../include/views/utils/StatesView.h"

void TrucoGame::View::CardButton::discardCardOnTheTable()
{	
	areCardsInTheHandsOfThePlayer = false;
	std::thread* animationThread;

	animationThread = new std::thread(&TrucoGame::View::Animator::moveAndRotateSpriteTo,
		std::ref(*card),
		discardOnTheTablePosition,
		90.0f,
		animationSpeed);

	animationThread->detach();

	delete animationThread;
}


void TrucoGame::View::CardButton::onPressLeft() 
{
	isPlayerTurnToPlayMutex.lock();
	if (isPlayerTurnToPlayState == IsPlayerTurnToPlayState::PlayerTurn) {
		isPlayerTurnToPlayState = IsPlayerTurnToPlayState::NotPlayerTurn;
		
		discardCardOnTheTable();
	}
	isPlayerTurnToPlayMutex.unlock();


	cardButtonClick(this->cardIndex, false);
}

void TrucoGame::View::CardButton::onPressRight()
{
	isPlayerTurnToPlayMutex.lock();
	if (isPlayerTurnToPlayState == IsPlayerTurnToPlayState::PlayerTurn) {
		isPlayerTurnToPlayState = IsPlayerTurnToPlayState::NotPlayerTurn;

		uIThreadMutex.lock();
		*cardTexture = UtilsView::loadTextureBack();
		uIThreadMutex.unlock();
		discardCardOnTheTable();
	}
	isPlayerTurnToPlayMutex.unlock();
	cardButtonClick(this->cardIndex, true);
}

void TrucoGame::View::CardButton::onHover() {

	isPlayerTurnToPlayMutex.lock();
	if (isPlayerTurnToPlayState == IsPlayerTurnToPlayState::PlayerTurn) {
		this->setOutlineThickness(4);
	}
	isPlayerTurnToPlayMutex.unlock();
}

void TrucoGame::View::CardButton::onIdle() {
	this->setOutlineThickness(0);
}

TrucoGame::View::CardButton::CardButton(
	float x, float y, float width, float height, Color hoverColor, Sprite* card, Vector2f& windowSize, float animationSpeed, Vector2f& discardOnTheTablePosition, Texture* cardTexture, size_t cardIndex) :
	ButtonBase(x, y, width, height)
{
	this->setFillColor(Color::Transparent);
	this->setOutlineColor(Color::Transparent);
	this->setOutlineColor(hoverColor);

	areCardsInTheHandsOfThePlayer = false;
	this->card = card;
	this->discardOnTheTablePosition = discardOnTheTablePosition;
	this->animationSpeed = animationSpeed;
	this->cardTexture = cardTexture;
	this->cardIndex = cardIndex;
}

TrucoGame::View::CardButton::~CardButton()
{
	delete cardTexture;
}

bool TrucoGame::View::CardButton::getAreCardsInTheHandsOfThePlayer() {
	return areCardsInTheHandsOfThePlayer;
}

void TrucoGame::View::CardButton::setAreCardsInTheHandsOfThePlayer(bool areCardsInTheHandsOfThePlayer) {
	this->areCardsInTheHandsOfThePlayer = areCardsInTheHandsOfThePlayer;
}