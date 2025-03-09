#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
  public:
    float min, max;

    interval() : min(+MY_INFINITY), max(-MY_INFINITY) {}

    interval(float min, float max) : min(min), max(max) {}

    float size() const {
        return max - min;
    }

    bool contains(float x) const {
        return min <= x && x <= max;
    }

    bool surrounds(float x) const {
        return min < x && x < max;
    }

    float clamp(float x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
 
    // static const interval empty, universe;
};

// const interval interval::empty    = interval(+MY_INFINITY, -MY_INFINITY);
// const interval interval::universe = interval(-MY_INFINITY, +MY_INFINITY);

#endif