#include "Ctx.h"

#include <JSON/printer.h>

#include "snd/Player.h"
#include "scn/Start.h"
#include "scn/Entry.h"
#include "scn/Beach.h"
#include "ecs/Stats.h"
#include "io/Out.h"
#include "ecs/Acceleration.h"

GameStateMachine::GameStateMachine() {
    this->current.push(StateStart);
}

GameState GameStateMachine::currentState() {
    return this->current.top();
}

void GameStateMachine::popState() {
    if (this->current.size() > 1) this->current.pop();
}

bool GameStateMachine::toggleMenu() {
    // Can't get out of the initial start menu by pressing esc
    if (this->current.top() == StateStart) return false;
    if (this->current.top() == StateSaving) return false;
    if (this->current.top() == StateLoading) return false;

    // In-game allow to toggle the menu
    if (this->current.top() == StateMainMenu) {
        Player::instance().resume();
        this->current.pop();
    } else {
        Player::instance().pause();
        this->current.push(StateMainMenu);
        return true;
    }
    return false;
}

void GameStateMachine::pushState(GameState state) {
    if (this->currentState() != state) {
        this->current.push(state);
    }
}

Ctx::Ctx() {
    scenes.emplace(SceneStart, std::make_shared<Entry>());
    scenes.emplace(SceneDungeon, std::make_shared<Start>());
    scenes.emplace(SceneBeach, std::make_shared<Beach>());
}

void Ctx::setPlayer(std::shared_ptr<Entity> player) {
    this->player = player;
}

void Ctx::setMenu(std::shared_ptr<Entity> menu) {
    this->menu = menu;
}

std::shared_ptr<Entity> Ctx::getPlayer() {
    return this->player;
}

std::shared_ptr<Entity> Ctx::getMenu() {
    return this->menu;
}

void Ctx::save(float x, float y) {
    auto stats = getPlayer()->getComponent<Stats>();
    auto acc = getPlayer()->getComponent<Acceleration>();
    JSON::Value to;
    to["x"] = x;
    to["y"] = y;
    to["direction"] = acc->getDirection();
    to["scene"] = activeScene->getSceneType();
    to["renderHints"] = Manager::instance().getRenderHints();
    stats->inventory.serialize(to);
    stats->character.serialize(to);
    this->memory.serialize(to);

    Out f("savegame.json");
    if (f.open()) {
        JSON::PrettyPrinter p;
        std::string result = p.print(to);
        f.write(result);

        if (!saved) {
            saved = new bool;
        }
        *saved = true;
    }
}

bool Ctx::savegameExists() {
    if (!saved) {
        File f("savegame.json");
        saved = new bool;
        *saved = f.open();
    }
    return saved;
}

void Ctx::load(float *x, float *y) {
    File f("savegame.json");
    if (f.open()) {
        std::string buffer;
        buffer.resize(f.size);
        f.read(buffer, 0, f.size);

        JSON::Value v;
        JSON::Parser p;
        p.parse(v, buffer);

        SceneType sceneType = static_cast<SceneType>(v["scene"].as<int>());
        setActiveScene(sceneType);

        this->memory.deserialize(v);
        auto stats = this->getPlayer()->getComponent<Stats>();
        stats->character.deserialize(v);
        stats->inventory.deserialize(v);

        *x = v["x"].as<float>();
        *y = v["y"].as<float>();

        uint8_t renderHints = static_cast<uint8_t>(v["renderHints"].as<int>());
        Manager::instance().setRenderHints(renderHints);
    }
}