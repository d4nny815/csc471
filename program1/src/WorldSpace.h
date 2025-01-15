#ifndef WORLDSPACE_H
#define WORLDSPACE_H

#include <stdio.h>

class ViewVolume {
public:
    ViewVolume() 
        : left(0.0f), right(0.0f), bottom(0.0f), 
        top(0.0f), near(0.0f), far(0.0f) {}

    ViewVolume(float left, float right, float bottom, 
                float top, float near, float far) 
            : left(left), right(right), bottom(bottom), 
                top(top), near(near), far(far) {}
    
    void calc_vvolume(int width, int height);

    float left, right, bottom, top, near, far;

    void print() {
        printf("vvolume(%f, %f, %f, %f)\n", left, right, bottom, top);
    }
};

#endif 