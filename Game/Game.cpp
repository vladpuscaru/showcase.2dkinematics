//
// Created by Vlad Puscaru on 25.11.2023.
//

#include "Game.h"

void Game::run() {
    m_window.setFramerateLimit(60);
    sf::Clock clock;
    ImGui::SFML::Init(m_window);

    while (m_window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        ImGui::SFML::Update(m_window, deltaTime);
        drawUI();

        input();
        update();
        render();
    }
}

void Game::input() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Q) {
            m_window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
            m_arm->setJointAngle(1, m_arm->getJointAngle(1) - 1.f);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
            m_arm->setJointAngle(1, m_arm->getJointAngle(1) + 1.f);
        }
    }
}

void Game::update() {
    armDance();
    configureArm();
    m_arm->forwardKinematics();
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    for (int i = 0; i < m_arm->m_config.size(); i++) {
        const auto &pair = m_arm->m_config[i];
        auto &link = std::get<Link>(pair);
        auto &joint = std::get<Joint>(pair);

        sf::VertexArray line(sf::LinesStrip, 2);
        line[0].position = link.m_start;
        line[0].color = m_config.linksColors[i];
        line[1].position = link.m_end;
        line[1].color = m_config.linksColors[i];

        m_window.draw(line);

        sf::CircleShape c(5);
        c.setOrigin(c.getRadius(), c.getRadius());
        c.setPosition(joint.m_position);
        c.setFillColor(m_config.jointColors[i]);

        m_window.draw(c);
    }

    sf::VertexArray line(sf::LinesStrip, 2);
    line[0].position = m_arm->m_bat.m_start;
    line[0].color = sf::Color::Blue;
    line[1].position = m_arm->m_bat.m_end;
    line[1].color = sf::Color::Blue;

    m_window.draw(line);

    ImGui::SFML::Render(m_window);

    m_window.display();
}

Game::Game()
        : m_window(sf::VideoMode(W_WIDTH, W_HEIGHT), W_TITLE) {
    init();
}

void Game::init() {
    // Start configuration
    m_config.links.push_back(100.f);
    m_config.linksColors.push_back(sf::Color::White);
    m_config.links.push_back(50.f);
    m_config.linksColors.push_back(sf::Color::White);
    m_config.links.push_back(30.f);
    m_config.linksColors.push_back(sf::Color::White);

    m_config.joints.push_back(0);
    m_config.jointColors.push_back(sf::Color::White);
    m_config.joints.push_back(0);
    m_config.jointColors.push_back(sf::Color::White);
    m_config.joints.push_back(0);
    m_config.jointColors.push_back(sf::Color::White);

    m_config.batLength = 35.f;

    m_config.startAngle = -90;
    m_config.startPosition = {(float) m_window.getSize().x / 2, (float) m_window.getSize().y};

    m_isDancing = false;

    configureArm();
}

void Game::drawUI() {
    ImGui::Begin("User Interface for Kinematic Arm");

    static int selectedTab = 0;

    if (ImGui::Button("Configuration")) {
        selectedTab = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Motions")) {
        selectedTab = 1;
    }
    ImGui::SameLine();
    if (ImGui::Button("Forward Kinematics")) {
        selectedTab = 2;
    }
    ImGui::SameLine();
    if (ImGui::Button("Inverse Kinematics")) {
        selectedTab = 3;
    }

    if (selectedTab == 0) {
        int noLinks = m_config.links.size();
        float baseX = m_config.startPosition.x;
        float baseY = m_config.startPosition.y;
        float baseTheta = m_config.startAngle;


        if (ImGui::SliderInt("Number of Links", &noLinks, 1, 15)) {
            reconfigure(noLinks, baseX, baseY, baseTheta);
        }

        ImGui::Text("Base Location");
        ImGui::SameLine();
        if (ImGui::SliderFloat("X", &baseX, 0, m_window.getSize().x)) {
            reconfigure(noLinks, baseX, baseY, baseTheta);
        }
        if (ImGui::SliderFloat("Y", &baseY, 0, m_window.getSize().y)) {
            reconfigure(noLinks, baseX, baseY, baseTheta);
        }

        if (ImGui::SliderFloat("Base Angle", &baseTheta, 0, 360)) {
            reconfigure(noLinks, baseX, baseY, baseTheta);
        }
    }

    if (selectedTab == 1) {
        if (ImGui::Button("Dance")) {
            m_isDancing = !m_isDancing;
        }
    }

    if (selectedTab == 2) {
        if (ImGui::CollapsingHeader("Links")) {
            for (int i = 0; i < m_config.links.size(); i++) {
                ImGui::PushID(i);
                float size = m_config.links[i];
                float color[] = {(float) m_config.linksColors[i].r / 255,
                                 (float) m_config.linksColors[i].g / 255,
                                 (float) m_config.linksColors[i].b / 255};

                ImGui::Text("Link %d", i);
                if (ImGui::SliderFloat("Size", &size, 1.0f, 150.f)) {
                    m_config.links[i] = size;
                }
                if (ImGui::CollapsingHeader("Color")) {
                    if (ImGui::ColorPicker3("Color", color)) {
                        m_config.linksColors[i].r = color[0] * 255;
                        m_config.linksColors[i].g = color[1] * 255;
                        m_config.linksColors[i].b = color[2] * 255;
                    }
                }
                ImGui::PopID();
            }
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Joints")) {
            for (int i = 0; i < m_config.joints.size(); i++) {
                ImGui::PushID(i);
                float angle = m_config.joints[i];
                float color[] = {(float) m_config.jointColors[i].r / 255,
                                 (float) m_config.jointColors[i].g / 255,
                                 (float) m_config.jointColors[i].b / 255};

                ImGui::Text("Link %d", i);
                if (ImGui::SliderFloat("Angle", &angle, 0.0f, 360.f)) {
                    m_config.joints[i] = angle;
                }
                if (ImGui::CollapsingHeader("Color")) {
                    if (ImGui::ColorPicker3("Color", color)) {
                        m_config.jointColors[i].r = color[0] * 255;
                        m_config.jointColors[i].g = color[1] * 255;
                        m_config.jointColors[i].b = color[2] * 255;
                    }
                }
                ImGui::PopID();
            }
        }
    }

    ImGui::End();
}

void Game::configureArm() {
    if (m_arm) {
        for (int i = 0; i < m_config.links.size(); i++) {
            m_arm->setLinkLength(i, m_config.links[i]);
        }
        for (int i = 0; i < m_config.joints.size(); i++) {
            m_arm->setJointAngle(i, m_config.joints[i]);
        }
        m_arm->setStartPosition(m_config.startPosition);
        m_arm->setBatLength(m_config.batLength);
    } else {
        m_arm = std::make_shared<Arm>(m_config.links.size(),
                                      m_config.startPosition,
                                      m_config.startAngle,
                                      m_config.batLength);
    }
}

void Game::armDance() {
    if (m_isDancing) {
        std::cout << m_config.joints[1] << std::endl;
        if (m_motions.empty()) {
            // Start motions for all the joints
            for (int i = 0; i < m_config.joints.size(); i++) {
                float start = i % 2 == 0 ? -90 : 90;
                float end = i % 2 == 0 ? 90 : -90;
                float step = i % 2 == 0 ? 2 : -2;

                m_motions.push_back(Motion(start, end, step));
                m_motions[i].m_reversed = i % 2 == 0 ? false : true;

                m_config.joints[i] = start;
            }
        } else {
            for (int i = 0; i < m_motions.size(); i++) {
                if (m_motions[i].m_finished) {
                    float temp = m_motions[i].m_startAngle;
                    m_motions[i].m_startAngle = m_motions[i].m_endAngle;
                    m_motions[i].m_endAngle = temp;
                    m_motions[i].m_stepSize = -m_motions[i].m_stepSize;
                    m_motions[i].m_finished = false;
                    m_motions[i].m_reversed = !m_motions[i].m_reversed;
                } else {
                    m_motions[i].update();
                }

                m_config.joints[i] = m_motions[i].m_currentAngle;
            }
        }
    } else if (!m_motions.empty()) {
        m_motions.clear();
    }
}

void Game::reconfigure(int noLinks, float baseX, float baseY, float baseAngle) {

    m_config.startPosition = { baseX, baseY };
    m_config.startAngle = baseAngle;

    if (noLinks > m_config.links.size()) {
        m_config.links.push_back(30.f);
        m_config.linksColors.push_back(sf::Color::White);
        m_config.joints.push_back(0);
        m_config.jointColors.push_back(sf::Color::White);
    } else {
        int count = 0;
        std::vector<float> links;
        std::vector<sf::Color> linksColors;
        std::vector<float> joints;
        std::vector<sf::Color> jointsColors;
        while (count < noLinks) {
            links.push_back(m_config.links[count]);
            linksColors.push_back(m_config.linksColors[count]);
            joints.push_back(m_config.joints[count]);
            jointsColors.push_back(m_config.jointColors[count]);
            count++;
        }
        m_config.links = links;
        m_config.linksColors = linksColors;
        m_config.joints = joints;
        m_config.jointColors = jointsColors;
    }

    m_arm = nullptr; // This will make configureArm() to recompute the arm based on the new values of m_config
}
