#include "camera.h"

#include "system/scr.h"

namespace geometry {

const geometry::GeoF panSpeed = 0.6;
const geometry::GeoF pitchSpeed = 0.6;

const geometry::GeoF travelSpeed = 0.2;

void Camera::tickFrame() {
    projFrame++;
}

void Camera::updateFromGamepad(uint16_t pad) {
    if (pad & INPUT_MASK_LEFT) {
        rotation.heading.adjust(-panSpeed);
    }
    if (pad & INPUT_MASK_RIGHT) {
        rotation.heading.adjust(panSpeed);
    }
    if (pad & INPUT_MASK_UP) {
        rotation.pitch.adjust(-pitchSpeed);
    } 
    if (pad & INPUT_MASK_DOWN) {
        rotation.pitch.adjust(pitchSpeed);
    }
    if (pad & INPUT_MASK_B) {
        Coord delta = rotation.apply({0.0, 0.0, -travelSpeed});
        position += delta;
    }
    if (pad & INPUT_MASK_A) {
        Coord delta = rotation.apply({0.0, 0.0, travelSpeed});
        position += delta;
    }

}

Coord Camera::project(const Coord &cc) const {
    Coord translated = cc - position;
    Coord rotated = rotation.apply(translated);
    Coord projected = mat.project(rotated);
    return projected;
}

}