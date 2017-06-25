#include "Iter_Window.h"
#include "gui/Button.h"
#include "gui/text.h"

Iter_Window::Iter_Window(sf::RenderWindow& window, sf::Font& font) :
    m_window(window),
    m_font(font),
    m_iterator(),
    m_input(&window, &font, 110, 30, 300, 15, "File"),
    m_saved(&window, &font, 460, 30, 200, 15, ""),
    m_elements(0),
    m_currentLevel(0)
{
    m_saved.SetOutlineColor(sf::Color::White);
    if(!m_window.isOpen()) {
        m_window.create(sf::VideoMode(1200, 724), "FractaSketch", sf::Style::Titlebar | sf::Style::Close);
        m_window.setPosition(sf::Vector2i(0, 0));
    }
    //TODO: Make relative to window & each other
    m_elements.push_back(new Button(&window, &font, 5, 5, 100, 15, "Level 0"));
    m_elements.push_back(new Button(&window, &font, 110, 5, 100, 15, "Level 1"));
    m_elements.push_back(new Button(&window, &font, 215, 5, 100, 15, "Level 2"));
    m_elements.push_back(new Button(&window, &font, 320, 5, 100, 15, "Level 3"));
    m_elements.push_back(new Button(&window, &font, 425, 5, 100, 15, "Level 4"));
    m_elements.push_back(new Button(&window, &font, 530, 5, 100, 15, "Level 5"));
    m_elements.push_back(new Button(&window, &font, 635, 5, 100, 15, "Level 6"));
    m_elements.push_back(new Button(&window, &font, 740, 5, 100, 15, "Level 7"));
    m_elements.push_back(new Button(&window, &font, 845, 5, 100, 15, "Level 8"));
    m_elements.push_back(new Button(&window, &font, 950, 5, 100, 15, "Level 9"));
    m_elements.push_back(new Button(&window, &font, 1055, 5, 100, 15, "Infinity"));
    m_elements.push_back(new Button(&window, &font, 5, 30, 100, 15, "Save"));
    m_window.close();
}

Iter_Window::~Iter_Window() {
    m_window.close();
    for(auto it = m_elements.begin(); it != m_elements.end(); it++) {
        delete (*it);
    }
    m_elements.clear();
}

void Iter_Window::HandleEvents() {
    if(!m_window.isOpen())
        return;
    sf::Event event;
    while(m_window.pollEvent(event)) {
        switch(event.type) {
        case sf::Event::Closed:
            m_window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if(event.mouseButton.button == sf::Mouse::Button::Left) {
                for(size_t iii = 0; iii <= ITERATOR_LEVELS + 1; iii++) {
                    if(m_elements[iii]->IsClicked(event.mouseButton.x, event.mouseButton.y)) {
                        m_elements[iii]->OnClick(event.mouseButton.x, event.mouseButton.y);
                        m_elements[m_currentLevel]->SetActive(false);
                        m_currentLevel = iii;
                        m_elements[m_currentLevel]->SetActive(true);
                        m_iterator.SetLevel(m_currentLevel);
                    }
                }
                // Save button
                if(m_elements[ITERATOR_LEVELS + 2]->IsClicked(event.mouseButton.x, event.mouseButton.y)) {
                    sf::RenderTexture tex;
                    tex.create(m_window.getSize().x, m_window.getSize().y);
                    tex.clear(sf::Color::White);
                    m_iterator.Draw(tex);
                    tex.display();

                    sf::Image img = tex.getTexture().copyToImage();
                    std::string filename = m_input.GetText();
                    if (img.saveToFile(filename))
                        m_saved.SetText("Image saved!");
                    else
                        m_saved.SetText("Save failed :(");
                }
                if(m_input.IsClicked(event.mouseButton.x, event.mouseButton.y)) {
                    m_input.OnClick(event.mouseButton.x, event.mouseButton.y);
                }
            }
            break;
        case sf::Event::KeyPressed:
            if(sf::Keyboard::Num0 < event.key.code &&
               event.key.code <= sf::Keyboard::Num9) {
                // formerly this would set level, but that interferes with saving to a file
                // UpdateLevel(event.key.code - sf::Keyboard::Num0);
            } else if(event.key.code == sf::Keyboard::Tilde) { // Level 0
                UpdateLevel(0);
            } else if(event.key.code == sf::Keyboard::Num0) { // Level 10
                UpdateLevel(10);
            } else {
                m_input.OnKeyPressed(event.key.code);
            }
            break;
        case sf::Event::TextEntered:
            m_input.OnTextEntered(event.text.unicode);
            break;
        default:
            break;
        }
    }
}

void Iter_Window::Draw() {
    if(!m_window.isOpen())
        return;
    m_window.clear(sf::Color::White);

    for(auto it = m_elements.begin(); it != m_elements.end(); it++) {
        (*it)->Draw();
    }
    m_input.Draw();
    m_saved.Draw();

    m_iterator.Draw(m_window);

    m_window.display();
}

void Iter_Window::StartNewIteration(Fractal_Template base) {
    if(!m_window.isOpen()) {
        m_window.create(sf::VideoMode(1200, 724), "FractaSketch", sf::Style::Titlebar | sf::Style::Close);
        m_window.setPosition(sf::Vector2i(0, 0));
    }
    m_elements[m_currentLevel]->SetActive(false);
    m_currentLevel = 0;
    m_elements[m_currentLevel]->SetActive(true);

    m_iterator.SetBase(base);
}

void Iter_Window::UpdateLevel(size_t newLevel) {
    m_elements[m_currentLevel]->SetActive(false);
    m_currentLevel = newLevel;
    m_elements[m_currentLevel]->SetActive(true);
    m_iterator.SetLevel(m_currentLevel);
}
