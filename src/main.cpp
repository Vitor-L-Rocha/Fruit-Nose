#include "Game.hpp"

int main() {
    cv::VideoCapture cap(0);

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    Game game;

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        cv::flip(frame, frame, 1);

        game.update(frame);

        cv::imshow("Fruit Nose", frame);

        int key = cv::waitKey(30);

        if (key == 27) break;   // ESC
        if (key == 13) game.start(); // ENTER
        if (key == 'r') game.restart();
    }
}