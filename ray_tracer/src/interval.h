#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
  public:
    float min, max;

    Interval() : min(+MY_INFINITY), max(-MY_INFINITY) {}

    Interval(float min, float max) : min(min), max(max) {}

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
 
    // static const Interval empty, universe;
};

// const Interval Interval::empty    = Interval(+MY_INFINITY, -MY_INFINITY);
// const Interval Interval::universe = Interval(-MY_INFINITY, +MY_INFINITY);

#endif