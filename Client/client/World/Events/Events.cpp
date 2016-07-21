#include "Events.hpp"



Events::Events()
{
    m_Movement = new Movement();
    m_DirectionMap[sf::Keyboard::Key::Up] = Orientation::Up;
    m_DirectionMap[sf::Keyboard::Key::Down] = Orientation::Down;
    m_DirectionMap[sf::Keyboard::Key::Left] = Orientation::Left;
    m_DirectionMap[sf::Keyboard::Key::Right] = Orientation::Right;
}


Events::~Events()
{
}

void Events::SetActualOrientation(Orientation p_Orientation)
{
    m_Movement->SetActualOrientation(p_Orientation);
}

Movement* Events::GetMovementHandler() const
{
    return m_Movement;
}

void Events::KeyRelease(sf::Keyboard::Key p_KeyRealease)
{
    std::vector<sf::Keyboard::Key>::iterator l_It = std::find(m_KeyPressed.begin(), m_KeyPressed.end(), p_KeyRealease);

    if (l_It != m_KeyPressed.end())
        m_KeyPressed.erase(l_It);

    if (m_KeyPressed.size() > 0)
    {
        m_Movement->SetActualOrientation((Orientation)m_DirectionMap[m_KeyPressed.back()]);
    }
    else
    {
        m_Movement->StopMovement();
    }
}

void Events::NewKeyPressed(sf::Keyboard::Key p_NewKey)
{
    switch (p_NewKey)
    {
        /// Direction
        case sf::Keyboard::Key::Up:
        case sf::Keyboard::Key::Down:
        case sf::Keyboard::Key::Left:
        case sf::Keyboard::Key::Right:
        {
            std::vector<sf::Keyboard::Key>::iterator l_It = std::find(m_KeyPressed.begin(), m_KeyPressed.end(), p_NewKey);

            if (l_It != m_KeyPressed.end())
                break;

            /// New Direction
            if (m_KeyPressed.size() > MAX_KEY_SAVE)
                m_KeyPressed.erase(m_KeyPressed.begin());
            m_KeyPressed.push_back(p_NewKey);
            SetActualOrientation((Orientation)m_DirectionMap[p_NewKey]);
            m_Movement->StartMovement();
            break;
        }
        default:
            break;
    }
}

