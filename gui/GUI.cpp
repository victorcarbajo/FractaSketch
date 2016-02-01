#include "gui.h"

GUI::GUI(sf::RenderWindow* window, sf::Font* inFont, Fractal_Iterator& iter):
    m_window(window),
    m_inFont(inFont),
    m_iter(iter),
    elements(0),
    line_topRight(window, inFont, 5, 5, 100, 15, "TopRight"),
    line_botRight(window, inFont, 110, 5, 100, 15, "BotRight"),
    line_topLeft(window, inFont, 215, 5, 100, 15, "TopLeft"),
    line_botLeft(window, inFont, 320, 5, 100, 15, "BotLeft"),
    line_static(window, inFont, 425, 5, 100, 15, "Static"),
    line_hidden(window, inFont, 530, 5, 100, 15, "Hidden"),
    grid_none(window, inFont, 635, 5, 100, 15, "No Grid"),
    grid_square(window, inFont, 740, 5, 100, 15, "Square Grid"),
    grid_hex(window, inFont, 845, 5, 100, 15, "Hex Grid"),
    draw_button(window, inFont, 950, 5, 100, 15, "Draw"),
    m_grid(window, sf::Vector2f(0, GUI_HEIGHT_OFFSET), sf::Vector2f(window->getSize()), Grid::gt_square)
{
    line_topRight.SetActive(true);
    elements.push_back(&line_topRight);
    elements.push_back(&line_botRight);
    elements.push_back(&line_topLeft);
    elements.push_back(&line_botLeft);
    elements.push_back(&line_static);
    elements.push_back(&line_hidden);
    elements.push_back(&grid_none);
    elements.push_back(&grid_square);
    elements.push_back(&grid_hex);
    elements.push_back(&draw_button);

    m_activeLine = 0;
}

GUI::~GUI() {

}

void GUI::Draw() {
    std::vector<GUI_Element*>::iterator it;
    for(it = elements.begin(); it != elements.end(); it++)
        (*it)->Draw();
    m_grid.Draw();
}

sf::Vector2f GUI::SnapToGrid(sf::Vector2f point) {
    return m_grid.SnapToNearest(point);
}

event_result GUI::HandleEvent(sf::Event event) {
    switch(event.type) {
    case sf::Event::Closed:
        m_window->close();
    case sf::Event::MouseButtonPressed:
        for(int iii = 0; iii < elements.size(); iii++) {
            if(elements[iii]->IsClicked(event.mouseButton.x, event.mouseButton.y)) {
                elements[iii]->OnClick(event.mouseButton.x, event.mouseButton.y);
                if(iii <= 5) {
                    elements[m_activeLine]->SetActive(false);
                    m_activeLine = iii;
                    elements[m_activeLine]->SetActive(true);
                }
                else if(iii < 9){
                    m_grid.SetType((Grid::grid_type)(iii-6));
                }
            }
        }
        return et_none;
    case sf::Event::KeyPressed:
        if(sf::Keyboard::Num1 <= event.key.code &&
           event.key.code <= sf::Keyboard::Num6) {
            elements[m_activeLine]->SetActive(false);
            m_activeLine = event.key.code - sf::Keyboard::Num1;
            elements[m_activeLine]->SetActive(true);
        }
    }
}

Line::line_type GUI::GetLineType() {
    return (Line::line_type)(m_activeLine+1);
}
