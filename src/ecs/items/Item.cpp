#include "Item.h"
#include "../Collectable.h"
#include "../Transform.h"
#include "../Collider.h"
#include "HealthPotion.h"
#include "../Analytics.h"

std::shared_ptr<Entity> Item::make(Position p, ItemType type) {
    std::shared_ptr<Item> item = nullptr;
    switch (type) {
        case EMPTY_SLOT:
            return nullptr;
        case HEALTH_POTION:
            item = std::make_shared<HealthPotion>();
    }

    return make(p, item);
}

std::shared_ptr<Entity> Item::make(Position p, std::shared_ptr<Item> item) {
    auto entity = std::make_shared<Entity>();

    Padding defaaultPadding = {0.5, 0.5, 0.5, 0.5};
    entity->addComponent<Transform>(p.x, p.y, defaaultPadding);
    auto transform = entity->getComponent<Transform>();

    entity->addComponent<Collider>(transform, CT_ITEM, defaaultPadding);
    entity->addComponent<Collectable>(item);
    entity->addComponent<Analytics>();

    return entity;
}