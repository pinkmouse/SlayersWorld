#include "Events.hpp"
#include "../../Global.hpp"

Events::Events()
{
    m_DirectionMap[sf::Keyboard::Key::Up] = Orientation::Up;
    m_DirectionMap[sf::Keyboard::Key::Down] = Orientation::Down;
    m_DirectionMap[sf::Keyboard::Key::Left] = Orientation::Left;
    m_DirectionMap[sf::Keyboard::Key::Right] = Orientation::Right;
    m_WritingField = nullptr;
}


Events::~Events()
{
}

void Events::KeyRelease(sf::Keyboard::Key p_KeyRealease)
{
    if (g_Player == nullptr)
        return;

    std::vector<sf::Keyboard::Key>::iterator l_It = std::find(m_KeyPressed.begin(), m_KeyPressed.end(), p_KeyRealease);

    if (l_It != m_KeyPressed.end()) ///< If it's a movement key Release
    {
        m_KeyPressed.erase(l_It);

        if (m_KeyPressed.size() > 0)
        {
            g_Socket->SendGoDirection((Orientation)m_DirectionMap[m_KeyPressed.back()]);
            g_Player->SetOrientation((Orientation)m_DirectionMap[m_KeyPressed.back()]);
            g_Player->StartMovement();
        }
        else
        {
            g_Socket->SendStopMovement();
            g_Player->GetMovementHandler()->StopMovement();
        }
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
            if (g_Player == nullptr)
                return;

            std::vector<sf::Keyboard::Key>::iterator l_It = std::find(m_KeyPressed.begin(), m_KeyPressed.end(), p_NewKey);

            if (l_It != m_KeyPressed.end())
                break;

            /// New Direction
            if (m_KeyPressed.size() > MAX_KEY_SAVE)
                m_KeyPressed.erase(m_KeyPressed.begin());
            m_KeyPressed.push_back(p_NewKey);
            g_Player->GetMovementHandler()->StartMovement((Orientation)m_DirectionMap[p_NewKey]);
            g_Player->SetOrientation((Orientation)m_DirectionMap[p_NewKey]);
            g_Socket->SendGoDirection((Orientation)m_DirectionMap[p_NewKey]);
            break;
        }
        /// Reset KeyPress queue when lost focus
        case sf::Event::LostFocus:
        {
            if (g_Player == nullptr)
                return;

            m_KeyPressed.clear();
            g_Player->GetMovementHandler()->StopMovement();
            break;
        }
        case sf::Keyboard::Key::Return:
        {
            if (m_WritingField == nullptr)
                return;

            if (m_WritingField->IsFieldOpen())
                m_WritingField->Close();
            else
                m_WritingField->Open();

            break;
        }
        default:
            break;
    }
}

void Events::TextEntered(sf::Uint32 p_Letter)
{
    if (!m_WritingField->IsFieldOpen())
        return;

    if (p_Letter >= 128) ///< ASCII
        return;

    switch (p_Letter)
    {
        case 8:
        {
            m_WritingField->RemoveLastLetter();
            break;
        }
        default:
            if (p_Letter > 31 && p_Letter < 127)
                m_WritingField->AddLetter(p_Letter);
            break;
    }
    /// TODO
}

void Events::SetWritingField(WritingField* p_WritingField)
{
    m_WritingField = p_WritingField;
}