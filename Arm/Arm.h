//
// Created by Vlad Puscaru on 06.01.2024.
//

#ifndef KINEMATICS_ARM_H
#define KINEMATICS_ARM_H

#include "../Joint/Joint.h"
#include "../Link/Link.h"
#include <tuple>
#include <cmath>

class Arm {
public:
    std::vector<std::tuple<Link, Joint>> m_config;
    Link m_bat;

    sf::Vector2f m_startPosition;
    float m_startAngle;

    Arm(int noLinks, const sf::Vector2f& startPos, float startAngle, float batLength);

    void forwardKinematics();

    void setJointAngle(int jointIdx, float theta);
    float getJointAngle(int jointIdx);

    void setLinkLength(int linkIdx, float length);

    void setStartPosition(const sf::Vector2f& position);

    void setBatLength(float length);
};


#endif //KINEMATICS_ARM_H
