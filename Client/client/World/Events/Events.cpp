#include "Events.hpp"
#include "../../Global.hpp"

Events::Events()
{
    m_DirectionMap[eKeyBoardAction::KeyBoardUp] = Orientation::Up;
    m_DirectionMap[eKeyBoardAction::KeyBoardDown] = Orientation::Down;
    m_DirectionMap[eKeyBoardAction::KeyBoardLeft] = Orientation::Left;
    m_DirectionMap[eKeyBoardAction::KeyBoardRight] = Orientation::Right;
    m_WritingField = nullptr;

    m_KeyUsableWhileDeath[eKeyBoardAction::KeyBoardUp] = true;
    m_KeyUsableWhileDeath[eKeyBoardAction::KeyBoardDown] = true;
    m_KeyUsableWhileDeath[eKeyBoardAction::KeyBoardLeft] = true;
    m_KeyUsableWhileDeath[eKeyBoardAction::KeyBoardRight] = true;
}


Events::~Events()
{
}

bool Events::IsKeyUsableWhileDeath(eKeyBoardAction p_Key)
{
    if (m_KeyUsableWhileDeath.find(p_Key) != m_KeyUsableWhileDeath.end())
        return m_KeyUsableWhileDeath[p_Key];
    return false;
}

void Events::Update()
{
    if (g_Player == nullptr)
        return;

    MovementHandler* l_MovementHandler = g_Player->GetMovementHandler();

    if (l_MovementHandler == nullptr)
        return;

    /// If death, attack key will not be able to use, so we need to stop attack if player is attaking
    if (g_Player->IsDeath() && l_MovementHandler->IsInAttack())
    {
        std::vector<eKeyBoardAction>::iterator l_It = std::find(m_KeyPressed.begin(), m_KeyPressed.end(), eKeyBoardAction::KeyBoardAutoAttack);
        if (l_It != m_KeyPressed.end())
        {
            m_KeyPressed.erase(l_It);
            g_Socket->SendStopAttack();
            l_MovementHandler->StopAttack();
        }
    }

    /// Need update to check if when attack is finish we have direction key press
    if (!l_MovementHandler->IsInMovement() && !l_MovementHandler->GetInColision())
    {
        if (m_KeyPressed.size() > 0) ///< Check the others action
        {
            switch (m_KeyPressed.back())
            {
                case eKeyBoardAction::KeyBoardUp:
                case eKeyBoardAction::KeyBoardDown:
                case eKeyBoardAction::KeyBoardLeft:
                case eKeyBoardAction::KeyBoardRight:
                {
                    g_Socket->SendGoDirection((Orientation)m_DirectionMap[m_KeyPressed.back()], g_Player->GetPosX(), g_Player->GetPosY());
                    g_Player->SetOrientation((Orientation)m_DirectionMap[m_KeyPressed.back()]);
                    g_Player->StartMovement();
                    break;
                }
            }
        }
    }
}

void Events::LostFocus()
{
    if (g_Player == nullptr)
        return;

    m_KeyPressed.clear();
    MovementHandler* l_MovementHandler = g_Player->GetMovementHandler();
    KeyRelease(eKeyBoardAction::KeyBoardUp);
    KeyRelease(eKeyBoardAction::KeyBoardDown);
    KeyRelease(eKeyBoardAction::KeyBoardLeft);
    KeyRelease(eKeyBoardAction::KeyBoardRight);
    KeyRelease(eKeyBoardAction::KeyBoardAutoAttack);
}

void Events::KeyRelease(eKeyBoardAction p_NewKey)
{
    if (p_NewKey == eKeyBoardAction::NoneAction)
        return;

    if (g_Player == nullptr)
        return;

    if (g_Player->IsDeath() && !IsKeyUsableWhileDeath(p_NewKey))
        return;

    std::vector<eKeyBoardAction>::iterator l_It = std::find(m_KeyPressed.begin(), m_KeyPressed.end(), p_NewKey);
    if (l_It != m_KeyPressed.end())
    {
        m_KeyPressed.erase(l_It);
        MovementHandler* l_MovementHandler = g_Player->GetMovementHandler();

        if (m_KeyPressed.size() > 0) ///< Check the others action
        {
            switch (m_KeyPressed.back())
            {
                case eKeyBoardAction::KeyBoardUp:
                case eKeyBoardAction::KeyBoardDown:
                case eKeyBoardAction::KeyBoardLeft:
                case eKeyBoardAction::KeyBoardRight:
                {
                    g_Socket->SendGoDirection((Orientation)m_DirectionMap[m_KeyPressed.back()], g_Player->GetPosX(), g_Player->GetPosY());
                    g_Player->SetOrientation((Orientation)m_DirectionMap[m_KeyPressed.back()]);
                    g_Player->StartMovement();
                    break;
                }
                case eKeyBoardAction::KeyBoardAutoAttack:
                {           
                    MovementHandler* l_MovementHandler = g_Player->GetMovementHandler();
                    if (!l_MovementHandler->IsInAttack())
                        l_MovementHandler->StartAttack();
                    break;
                }
            }
        }
        else if ((p_NewKey == eKeyBoardAction::KeyBoardDown ||
            p_NewKey == eKeyBoardAction::KeyBoardUp ||
            p_NewKey == eKeyBoardAction::KeyBoardLeft ||
            p_NewKey == eKeyBoardAction::KeyBoardRight)) ///< If the last key release
        {
            g_Socket->SendStopMovement(l_MovementHandler->GetPosX(), l_MovementHandler->GetPosY());
            g_Player->GetMovementHandler()->StopMovement();
        }
        if (p_NewKey == eKeyBoardAction::KeyBoardAutoAttack)
        {
            g_Socket->SendStopAttack();
            l_MovementHandler->StopAttack();
        }
    }
}

void Events::NewKeyPressed(eKeyBoardAction p_NewKey)
{
    if (p_NewKey == eKeyBoardAction::NoneAction)
        return;

    if (g_Player == nullptr)
        return;

    if (g_Player->IsDeath() && !IsKeyUsableWhileDeath(p_NewKey))
        return;

    if (m_WritingField->IsFieldOpen() && p_NewKey != eKeyBoardAction::KeyBoardEnter && !IsKeyUsableWhileDeath(p_NewKey))
        return;

    switch (p_NewKey)
    {
        /// Direction
        case eKeyBoardAction::KeyBoardUp:
        case eKeyBoardAction::KeyBoardDown:
        case eKeyBoardAction::KeyBoardLeft:
        case eKeyBoardAction::KeyBoardRight:
        {
            if (g_Player == nullptr)
                return;

            MovementHandler* l_MovementHandler = g_Player->GetMovementHandler();

            std::vector<eKeyBoardAction>::iterator l_It = std::find(m_KeyPressed.begin(), m_KeyPressed.end(), p_NewKey);

            if (l_It != m_KeyPressed.end())
                break;

            /// New Direction
            if (m_KeyPressed.size() > MAX_KEY_SAVE)
                m_KeyPressed.erase(m_KeyPressed.begin());
            m_KeyPressed.push_back(p_NewKey);

            g_Player->GetMovementHandler()->StartMovement((Orientation)m_DirectionMap[p_NewKey]);
            g_Player->SetOrientation((Orientation)m_DirectionMap[p_NewKey]);
            g_Socket->SendGoDirection((Orientation)m_DirectionMap[p_NewKey], g_Player->GetPosX(), g_Player->GetPosY());
            break;
        }
        /// Attack
        case eKeyBoardAction::KeyBoardAutoAttack:
        {
            MovementHandler* l_MovementHandler = g_Player->GetMovementHandler();

            if (l_MovementHandler->IsInAttack())
                return;

            /*g_Socket->SendStopMovement(l_MovementHandler->GetPosX(), l_MovementHandler->GetPosY());
            l_MovementHandler->StopMovement();
            if (m_KeyPressed.size() > MAX_KEY_SAVE)
                m_KeyPressed.erase(m_KeyPressed.begin());*/
            m_KeyPressed.push_back(p_NewKey);

            g_Socket->SendStartAttack(g_Player->GetPosX(), g_Player->GetPosY());
            l_MovementHandler->StartAttack();
            break;
        }
        /// Action
        case eKeyBoardAction::KeyBoardSpell0:
        case eKeyBoardAction::KeyBoardSpell1:
        {
            g_Socket->SendEventAction((uint8)p_NewKey);
            break;
        }
        case eKeyBoardAction::KeyBoardAction:
        {
            g_Socket->SendEventAction((uint8)p_NewKey);
            break;
        }
        case eKeyBoardAction::KeyBoardEnter:
        {
            if (m_WritingField == nullptr)
                return;

            if (m_WritingField->IsFieldOpen())
                m_WritingField->Close();
            else
                m_WritingField->Open();

            break;
        }
        case eKeyBoardAction::KeyBoardHistoryMsg:
        {
            if (m_HistoryField == nullptr)
                return;

            if (m_HistoryField->IsFieldOpen())
            {
                if (m_HistoryField->GetLineHistory() == MIN_HISTORY_LINE)
                    m_HistoryField->SetLineHistory(MAX_HISTORY_LINE);
                else
                {
                    m_HistoryField->SetLineHistory(MIN_HISTORY_LINE);
                    m_HistoryField->Close();
                }
            }
            else
                m_HistoryField->Open();

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

    if (p_Letter >= 254) ///< ASCII
        return;

    switch (p_Letter)
    {
        case 8:
        {
            m_WritingField->RemoveLastLetter();
            break;
        }
        default:
            if (p_Letter > 31 && p_Letter < 254)
                m_WritingField->AddLetter(p_Letter);
            break;
    }
    /// TODO
}

void Events::SetWritingField(WritingField* p_WritingField)
{
    m_WritingField = p_WritingField;
}

void Events::SetHistoryField(HistoryField* p_HistoryField)
{
    m_HistoryField = p_HistoryField;
}