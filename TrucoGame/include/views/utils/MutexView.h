#ifndef MUTEX_VIEW_H
#define MUTEX_VIEW_H

#include <mutex>

extern std::mutex uIThreadMutex;
extern std::mutex distributeCardsToPlayersMutex;
extern std::mutex isPlayerTurnToPlayMutex;
extern std::mutex roundAndTurnMutex;
extern std::mutex discardCardMutex;
extern std::mutex cardMutex;
extern std::mutex playerIdMutex;
extern std::mutex cardStructMutex;
extern std::mutex roundScoreMutex;

#endif // MUTEX_VIEW_H
