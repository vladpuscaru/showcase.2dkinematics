//
// Created by Vlad Puscaru on 06.01.2024.
//

#ifndef KINEMATICS_MOTION_H
#define KINEMATICS_MOTION_H


class Motion {
public:
    float m_startAngle;
    float m_endAngle;
    float m_stepSize;

    float m_currentAngle;

    bool m_finished;
    bool m_reversed;

    void update();

    Motion(float start, float end, float step);
};


#endif //KINEMATICS_MOTION_H
