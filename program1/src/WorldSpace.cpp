#include "WorldSpace.h"

void ViewVolume::calc_vvolume(int width, int height) {
    if (width >= height) {
        right = static_cast<float>(width) / height;
        left = static_cast<float>(-width) / height;
        top = 1;
        bottom = -1;
        far = -1;
        near = 1;
    } else {
        top = static_cast<float>(height) / width;
        bottom = static_cast<float>(-height) / width;
        left = -1;
        right = 1;
        far = -1;
        near = 1;
    }

    return;
}