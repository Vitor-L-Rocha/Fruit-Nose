#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

class NoseTracker {
private:
    cv::CascadeClassifier face;
    cv::CascadeClassifier nose;

    cv::Point smoothNose = {-1, -1};

public:
    NoseTracker() {
        if (!face.load("assets/haarcascade_frontalface_default.xml")) {
            std::cout << "Erro ao carregar face\n";
            exit(1);
        }

        if (!nose.load("assets/haarcascade_mcs_nose.xml")) {
            std::cout << "Erro ao carregar nariz\n";
            exit(1);
        }
    }

    cv::Point detect(cv::Mat &frame) {
        std::vector<cv::Rect> faces;
        cv::Mat gray;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        face.detectMultiScale(gray, faces, 1.3, 5);

        if (faces.empty()) return smoothNose;

        cv::Rect f = faces[0];

        // ROI MAIOR (melhor detecção)
        cv::Rect roi(
            f.x,
            f.y + f.height * 0.3,
            f.width,
            f.height * 0.6
        );

        roi &= cv::Rect(0, 0, frame.cols, frame.rows);

        std::vector<cv::Rect> noses;
        nose.detectMultiScale(gray(roi), noses, 1.3, 5);

        cv::Point current;

        if (!noses.empty()) {
            cv::Rect n = noses[0];

            current = cv::Point(
                roi.x + n.x + n.width/2,
                roi.y + n.y + n.height/2
            );

            // evitar lixo muito pequeno
            if (n.width < 10 || n.height < 10) {
                return smoothNose;
            }
        }
        else {
            // fallback
            current = cv::Point(
                f.x + f.width/2,
                f.y + f.height * 0.65
            );
        }

        // 🔥 FILTRO ANTI-SALTO
        if (smoothNose.x != -1) {
            int dx = current.x - smoothNose.x;
            int dy = current.y - smoothNose.y;

            int maxJump = 80;

            if (abs(dx) > maxJump || abs(dy) > maxJump) {
                return smoothNose;
            }
        }

        // 🔥 SUAVIZAÇÃO MELHOR (mais responsivo)
        if (smoothNose.x != -1) {
            float alpha = 0.7;

            smoothNose.x = (1-alpha)*smoothNose.x + alpha*current.x;
            smoothNose.y = (1-alpha)*smoothNose.y + alpha*current.y;
        } else {
            smoothNose = current;
        }

        // debug visual
        cv::circle(frame, smoothNose, 15, {255,0,0}, -1);

        return smoothNose;
    }
};