//
// Created by Vlad Puscaru on 06.01.2024.
//

#ifndef KINEMATICS_JOINT_H
#define KINEMATICS_JOINT_H

#include <SFML/Graphics.hpp>

class Joint {
public:
    sf::Vector2f m_position;
    float m_theta;

    Joint(const sf::Vector2f &position, float theta);
};


#endif //KINEMATICS_JOINT_H
