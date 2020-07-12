#include <iostream>
#include "Skeleton.h"

Skeleton::Skeleton(Entity &parent) : Mind(parent) {}

Skeleton::~Skeleton() noexcept {}

int Skeleton::delay() {
    return 500;
}

void Skeleton::collide(std::shared_ptr<Collider> other) {

}

void Skeleton::plan(float dt) {
}
