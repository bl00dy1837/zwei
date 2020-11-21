#include "Collider.h"

#include "Entity.h"
#include "Acceleration.h"
#include "Ai.h"

Collider::Collider(Entity &parent, std::shared_ptr<Transform> tracked, ColliderTag tag,
                   float dx, float dy) :
        Component(parent),
        tracked(tracked),
        tag(tag),
        dx(dx),
        dy(dy) {
    updateBoundingBox();
}

void Collider::updateBoundingBox() {
    RT_Camera.project(boundingBox, tracked->p.x, tracked->p.y, dx, dy);
}

void Collider::update(float dt) {
    updateBoundingBox();
}

// Stop a mobile target
void Collider::stop(const Collider &c) {
    if (c.parent.hasComponent<Acceleration>()) {
        auto acceleration = c.parent.getComponent<Acceleration>();
        acceleration->decelerate();
        acceleration->reset(c.tracked->p);
    }
}

// Notify an AI-enabled entity that it collided with something
void Collider::notify(std::shared_ptr<Collider> other) {
    if (this->parent.hasComponent<Ai>()) {
        auto ai = this->parent.getComponent<Ai>();
        ai->collide(other);
    }
}


void Collider::collide(std::shared_ptr<Collider> other) {
    // Player / Wall collisions: stop player
    if (this->tag == CT_PLAYER) {
        if (other->tag == CT_WALL) {
            stop(*this);
        }
        return;
    }

    // Enemy / <any> collisions: inform about the collision with <other>
    if (this->tag == CT_ENEMY) {
        notify(other);
    }
}

