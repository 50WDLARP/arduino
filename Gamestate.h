#ifndef LARP_GAMESTATE
#define LARP_GAMESTATE

#include <Arduino.h>
typedef enum GameState {
    IT,
    NOT_IT,
    FROZEN,
    NOT_PLAYING
};
class TagGameStateManager {
public:
    void loop();
    void setup();
    GameState getState();
    void beTagged();
    void beUnTagged();
    void addTagger(bool (*tagged)(void));
    void addUnTagger(bool (*untagged)(void));
    void addItter(bool (*it)(void));
    void addQuitter(bool (*quit)(void));
private:
    GameState state;
    bool (*checkTagged)(void);
    bool (*checkUnTagged)(void);
    bool (*checkIt)(void);
    bool (*checkNotPlaying)(void);
};

#endif
