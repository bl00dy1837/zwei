#ifndef ZWEI_API_H
#define ZWEI_API_H

#include "ecs/Trigger.h"
#include "ecs/Interactible.h"

namespace Api {
    // Init
    void init();

    void initPlayer();

    void setPlayerSpeed(float maxSpeed);

    void setPlayerPosition(float x, float y);

    void getPlayerPosition(float &x, float &y);

    void setPlayerStats(int hp, int str, int dex, int def);

    // State
    void setMenuState();

    void setGameState();

    void setGameOverState();

    // Maps
    bool loadMap(const char *file);

    void unloadMap();

    void setRoofHideTrigger(int entryX, int entryY);

    void setRoofShowTrigger(int entryX, int entryY);

    void setDoor(int x, int y);

    void setTrigger(int x, int y, trigger_Fn onEnter, trigger_Fn onExit);

    void setInteractible(int x, int y, interact_Fn onInteract);

    void createSpeechBubble(const char *text);

    void createSingleSpeechBubble(const char *text, bool more);


    // Effects

    void setEnableLights(bool enabled);

}

#endif //ZWEI_API_H
