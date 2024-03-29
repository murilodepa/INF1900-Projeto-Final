#ifndef CARD_DECK_H
#define CARD_DECK_H

#pragma once
#include <SFML/Graphics.hpp>
#include "../CardView.h"

using namespace sf;

namespace TrucoGame {
    namespace View {
        class PlayerCards {
        private:
            Texture*** cardsTexture;
            size_t numPlayers;
            size_t quantityOfCardsInHands;
        public:
            CardView** cardsInHands;

            PlayerCards(size_t numPlayer, size_t quantityOfCardsInHands, Vector2f& initialDeckPositionVector2f, const float cardScale);
            ~PlayerCards();

            Texture* getCardTexture(size_t player, size_t cardIndex);

            void initializePlayerCards(Vector2f& initialDeckPositionVector2f, const float cardScale);
        };
    }
}

#endif // CARD_DECK_H