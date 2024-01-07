//
// Created by Vlad Puscaru on 06.01.2024.
//

#include "Arm.h"

/**
 * Sets correct position for links and joints based on joint angles and links lengths
 */
void Arm::forwardKinematics() {
    float angle = m_startAngle * (float) M_PI / 180; // angle must be in radians for std::cos, std::sin
    sf::Vector2f lastEndpoint = m_startPosition;
    for (auto &pair: m_config) {
        auto &link = std::get<Link>(pair);
        auto &joint = std::get<Joint>(pair);

        link.m_start = lastEndpoint;

        sf::Vector2f vec = { link.m_length, 0 };
        float rotatedX = std::cos(angle) * vec.x - std::sin(angle) * vec.y;
        float rotatedY = std::sin(angle) * vec.x + std::cos(angle) * vec.y;
        vec = { rotatedX, rotatedY };

        sf::Vector2f linkEnd = link.m_start + vec;

        joint.m_position = linkEnd;
        link.m_end = linkEnd;

        lastEndpoint = linkEnd;
        angle += joint.m_theta * (float) M_PI / 180; // angle must be in radians for std::cos, std::sin
    }

    m_bat.m_start = lastEndpoint;
    sf::Vector2f vec = { m_bat.m_length, 0 };
    float rotatedX = std::cos(angle) * vec.x - std::sin(angle) * vec.y;
    float rotatedY = std::sin(angle) * vec.x + std::cos(angle) * vec.y;
    vec = { rotatedX, rotatedY };

    sf::Vector2f linkEnd = m_bat.m_start + vec;
    m_bat.m_end = linkEnd;
}

Arm::Arm(int noLinks, const sf::Vector2f &startPos, float startAngle, float batLength)
    : m_startAngle(startAngle), m_startPosition(startPos), m_bat({0, 0}, batLength)
{
    for (int i = 0; i < noLinks; i++) {
        Link l({0.0f, 0.0f}, 50.f);
        Joint j({0.0f, 0.0f}, 0);
        m_config.push_back(std::make_pair(l, j));
    }

    forwardKinematics();
}

void Arm::setJointAngle(int jointIdx, float theta) {
    std::get<Joint>(m_config[jointIdx]).m_theta = theta;
}

float Arm::getJointAngle(int jointIdx) {
    return std::get<Joint>(m_config[jointIdx]).m_theta;
}

void Arm::setLinkLength(int linkIdx, float length) {
    std::get<Link>(m_config[linkIdx]).m_length = length;
}

void Arm::setStartPosition(const sf::Vector2f &position) {
    m_startPosition = position;
}

void Arm::setBatLength(float length) {
    m_bat.m_length = length;
}
