#include "Fractal_Iterator.h"

Fractal_Iterator::Fractal_Iterator(sf::RenderWindow* window) :
    m_window(window),
    m_levels(0)
{
    SetBase(Fractal_Element());
};

Fractal_Iterator::~Fractal_Iterator() {

}

void Fractal_Iterator::Draw() {
    m_levels[m_currentLevel].Draw(*m_window, true);
}

void Fractal_Iterator::SetBase(Fractal_Element newBase) {
    m_levels.clear();
    m_levels.push_back(newBase);
    m_currentLevel = 0;
}

void Fractal_Iterator::SetLevel(size_t level) {
    if(level > ITERATOR_LEVELS)
        level = ITERATOR_LEVELS - 1; // No infinity yet
    m_currentLevel = level;
    IterateTo(level);
}
#include <iostream>
void Fractal_Iterator::IterateTo(size_t level) {
    if(level < m_levels.size())
        return;
    for(size_t iii = m_levels.size(); iii <= level; iii++) {
        std::cout << "FOO\n";
        Fractal_Element nextFE = m_levels[0].ReplaceAll(m_levels[iii - 1]);
        m_levels.push_back(nextFE);
    }
}