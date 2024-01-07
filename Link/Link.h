//
// Created by Vlad Puscaru on 06.01.2024.
//

#ifndef KINEMATICS_LINK_H
#define KINEMATICS_LINK_H

#include <SFML/Graphics.hpp>

class Link {
public:
    sf::Vector2f m_start;
    sf::Vector2f m_end;
    float m_length;

    Link(const sf::Vector2f& start, float length);
};


#endif //KINEMATICS_LINK_H
