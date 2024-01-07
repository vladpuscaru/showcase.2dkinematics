//
// Created by Vlad Puscaru on 06.01.2024.
//

#include "Link.h"

Link::Link(const sf::Vector2f &start, float length)
        : m_start(start), m_length(length), m_end({start.x + length, start.y}) {

}
