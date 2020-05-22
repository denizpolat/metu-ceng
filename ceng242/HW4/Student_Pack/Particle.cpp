#include "Particle.h"

Particle::Particle(float x, float y) {
    this->x = x;
    this->y = y;
}

Particle::Particle(const Particle& p) {
    this->x = p.x;
    this->y = p.y;
}

float Particle::X() const {
    return this->x;
}

float Particle::Y() const {
    return this->y;
}

float Particle::FindDistance(const Particle& particle) const {
    float xdif = particle.x - this->x;
    float ydif = particle.y - this->y;
    float result = sqrt(pow(xdif,2) + pow(ydif, 2));
    return result;
}

bool Particle::operator== (const Particle& p) const {
    bool compx = fabs(p.x - this->x) <= EPSILON;
    bool compy = fabs(p.y - this->y) <= EPSILON;
    return (compx && compy);
}

ostream& operator<<(ostream& os, const Particle& particle) {
    os << "(" << particle.x  << "," << particle.y << ")";
    return os;
}