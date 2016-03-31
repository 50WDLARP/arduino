#include "Gamestate.h"

GameState TagGameStateManager::getState() {
    return state;
}


void TagGameStateManager::loop() {
    switch(state) {
        case NOT_IT:
            if ((*checkTagged)()) {
                beTagged();
            }
            break;
        case IT:
            if ((*checkUnTagged)()) {
                beUnTagged();
            }
            break;
        case FROZEN:
            if ((*checkUnTagged)()) {
                beUnTagged();
            }

            break;
    }
}

void TagGameStateManager::setup() {
    state = IT;
}

void TagGameStateManager::beTagged() {
    Serial.println("TAGGED");
    state = FROZEN;
}
void TagGameStateManager::beUnTagged() {
    Serial.println("TAGGED");
    state = NOT_IT;
}

void TagGameStateManager::addTagger(bool (*tagged)(void)) {
    checkTagged = tagged;
}
void TagGameStateManager::addUnTagger(bool (*tagged)(void)) {
    checkUnTagged = tagged;
}
