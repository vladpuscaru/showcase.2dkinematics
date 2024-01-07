//
// Created by Vlad Puscaru on 25.11.2023.
//

#ifndef KINEMATICS_GAME_H
#define KINEMATICS_GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Arm/Arm.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "../Motion/Motion.h"

#define W_WIDTH 1920
#define W_HEIGHT 720
#define W_TITLE "Kinematics"


struct Config {
    std::vector<float> links; // links lengths
    std::vector<sf::Color> linksColors;

    std::vector<float> joints; // joints angles
    std::vector<sf::Color> jointColors;

    float batLength;
    float startAngle;
    sf::Vector2f startPosition;
};


class Game {
private:
    sf::RenderWindow m_window;

    std::shared_ptr<Arm> m_arm;

    Config m_config;

    std::vector<Motion> m_motions;
    bool m_isDancing;

    void input();
    void update();
    void render();

    void drawUI();
    void init();
    void configureArm();
    void reconfigure(int noLinks, float baseX, float baseY, float baseAngle);

    void armDance();
public:
    Game();
    void run();
};


#endif //KINEMATICS_GAME_H
