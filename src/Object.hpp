// Object.hpp
#pragma once
#include <opencv2/opencv.hpp>

class Object {
public:
    cv::Point pos;
    int radius;
    int speed;
    bool alive;

    Object(int x, int y, int r, int s)
        : pos(x, y), radius(r), speed(s), alive(true) {}

    virtual ~Object() {}

    virtual void update() {
        pos.y -= speed;
    }

    virtual void draw(cv::Mat &frame) = 0;
};