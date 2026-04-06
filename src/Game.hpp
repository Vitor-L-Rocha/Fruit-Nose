#pragma once
#include <vector>
#include <cstdlib>
#include <fstream>
#include "Fruit.hpp"
#include "Bomb.hpp"
#include "NoseTracker.hpp"
#include "AudioManager.hpp"

enum GameState { MENU, PLAYING, GAME_OVER };

class Game {
private:
    std::vector<Object*> objects;
    NoseTracker tracker;
    AudioManager audio;

    int score = 0;
    int highScore = 0;
    bool newRecord = false;

    bool gameOver = false;
    GameState state = MENU;

public:

    // =========================
    // CONSTRUTOR
    // =========================
    Game() {
        loadHighScore();
    }

    // =========================
    // HIGH SCORE
    // =========================
    void loadHighScore() {
        std::ifstream file("highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        }
    }

    void saveHighScore() {
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

    // =========================
    // CONTROLE DE ESTADO
    // =========================
    void start() {
        state = PLAYING;
        score = 0;
        newRecord = false;
        gameOver = false;
        clearObjects();
    }

    void restart() {
        start();
    }

    void clearObjects() {
        for (auto o : objects) delete o;
        objects.clear();
    }

    // =========================
    // SPAWN
    // =========================
    void spawn(int width, int height) {
        int margin = width * 0.2;
        int x = margin + rand() % (width - 2*margin);

        if (rand()%20 == 0) {
            objects.push_back(new Fruit(x, height,
                "assets/fruits/apple.png"));
        }

        if (rand()%100 == 0) {
            objects.push_back(new Bomb(x, height));
        }
    }

    // =========================
    // UPDATE PRINCIPAL
    // =========================
    void update(cv::Mat &frame) {

        // =========================
        // MENU
        // =========================
        if (state == MENU) {
            cv::putText(frame, "FRUIT NOSE",
                {150,150}, cv::FONT_HERSHEY_SIMPLEX, 2,
                {0,255,0}, 3);

            cv::putText(frame, "Pressione ENTER para jogar",
                {100,250}, cv::FONT_HERSHEY_SIMPLEX, 1,
                {255,255,255}, 2);

            cv::putText(frame, "Recorde: " + std::to_string(highScore),
                {150,320}, cv::FONT_HERSHEY_SIMPLEX, 1,
                {255,255,0}, 2);

            return;
        }

        // =========================
        // GAME OVER
        // =========================
        if (state == GAME_OVER) {
            cv::putText(frame, "GAME OVER",
                {150,150}, cv::FONT_HERSHEY_SIMPLEX, 2,
                {0,0,255}, 3);

            cv::putText(frame, "Pressione R para reiniciar",
                {100,250}, cv::FONT_HERSHEY_SIMPLEX, 1,
                {255,255,255}, 2);

            cv::putText(frame, "Score: " + std::to_string(score),
                {150,300}, cv::FONT_HERSHEY_SIMPLEX, 1,
                {255,255,255}, 2);

            cv::putText(frame, "Recorde: " + std::to_string(highScore),
                {150,350}, cv::FONT_HERSHEY_SIMPLEX, 1,
                {255,255,0}, 2);

            if (newRecord) {
                cv::putText(frame, "NOVO RECORDE!",
                    {120,420}, cv::FONT_HERSHEY_SIMPLEX, 1.5,
                    {0,255,255}, 3);
            }

            return;
        }

        // =========================
        // JOGO RODANDO
        // =========================
        cv::Point nose = tracker.detect(frame);

        int collisionRadius = 35;

        if (nose.x != -1) {
            cv::circle(frame, nose, collisionRadius, {255,255,0}, 2);
        }

        spawn(frame.cols, frame.rows);

        for (auto &o : objects) {
            if (!o->alive) continue;

            o->update();
            o->draw(frame);

            if (nose.x != -1) {
                double dist = cv::norm(o->pos - nose);

                if (dist < (o->radius + collisionRadius)) {
                    o->alive = false;

                    if (dynamic_cast<Bomb*>(o)) {
                        audio.playExplosion();
                        gameOver = true;
                        state = GAME_OVER;
                    } else {
                        audio.playSlice();
                        score++;

                        // 🔥 ATUALIZA RECORD
                        if (score > highScore) {
                            highScore = score;
                            newRecord = true;
                            saveHighScore();
                        }
                    }
                }
            }
        }

        // =========================
        // HUD
        // =========================
        cv::putText(frame, "Score: " + std::to_string(score),
            {10,30}, cv::FONT_HERSHEY_SIMPLEX, 1,
            {0,0,255}, 2);
    }

    // =========================
    // DESTRUTOR
    // =========================
    ~Game() {
        clearObjects();
    }
};