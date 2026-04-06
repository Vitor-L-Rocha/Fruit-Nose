#pragma once
#include "Object.hpp"
#include <opencv2/opencv.hpp>

class Fruit : public Object {
private:
    cv::Mat sprite;

public:
    Fruit(int x, int y, std::string imgPath)
        : Object(x, y, 30, 5) {

        sprite = cv::imread(imgPath, cv::IMREAD_UNCHANGED);

        if (!sprite.empty()) {
            cv::resize(sprite, sprite, cv::Size(60, 60), 0, 0, cv::INTER_AREA);
        }
    }

    void draw(cv::Mat &frame) override {
        if (!alive || sprite.empty()) return;

        for (int i = 0; i < sprite.rows; i++) {
            for (int j = 0; j < sprite.cols; j++) {

                cv::Vec4b px = sprite.at<cv::Vec4b>(i, j);
                float alpha = px[3] / 255.0f;

                int x = pos.x + j - sprite.cols / 2;
                int y = pos.y + i - sprite.rows / 2;

                if (x >= 0 && y >= 0 &&
                    x < frame.cols && y < frame.rows) {

                    cv::Vec3b &bg = frame.at<cv::Vec3b>(y, x);

                    for (int c = 0; c < 3; c++) {
                        bg[c] = bg[c] * (1.0f - alpha) + px[c] * alpha;
                    }
                }
            }
        }
    }
};