//
// Created by Vlad Puscaru on 06.01.2024.
//

#include "Motion.h"

void Motion::update() {
    if (!m_finished) {
        m_currentAngle += m_stepSize;
        if ((!m_reversed && m_currentAngle >= m_endAngle) || (m_reversed && m_currentAngle <= m_endAngle)) {
            m_currentAngle = m_endAngle;
            m_finished = true;
        }
    }
}

Motion::Motion(float start, float end, float step)
    : m_startAngle(start), m_endAngle(end), m_stepSize(step), m_currentAngle(start), m_finished(false), m_reversed(false)
{

}
