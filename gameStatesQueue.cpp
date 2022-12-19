#include "gameStatesQueue.h"
#include <stdio.h>

GameState
GameStatesQueue::popFront()
{
    if (m_queueStates.size() == 0)
        return undefined;
    GameState state = m_queueStates.front();
    m_queueStates.pop();
    countDequeue++;
    return state;
}

GameState
GameStatesQueue::popFront(GameState gs)
{
    if (m_queueStates.size() == 0)
        return gs;
    GameState state = m_queueStates.front();
    m_queueStates.pop();
    countDequeue++;
    return state;
}

void
GameStatesQueue::pushBack(GameState gs)
{
    countEnqueue++;
    m_queueStates.push(gs);
}

void
GameStatesQueue::debugQueueSize()
{
    int qs = (int) m_queueStates.size();
    if (qs > 0)
        printf(
          "Game states size:%d (%d-%d)\n", qs, countEnqueue, countDequeue);
}
