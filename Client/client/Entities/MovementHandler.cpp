#include "MovementHandler.hpp"
#include "../Map/Map.hpp"

MovementHandler::MovementHandler(uint8 p_SizeX, uint8 p_SizeY) :
    m_SizeX(p_SizeX),
    m_SizeY(p_SizeY)
{
    m_Speed = 1.0f;
    m_InMovement = false;
    m_InAttack = false;
    m_StopAttack = false;
    m_InColision = false;
    m_AnimPeriodAsc = true;
    m_Orientation = Orientation::Down;
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_DiffTimeAnim = 0;
    m_DiffTimeAnimAttack = 0;
    m_DiffTime = 0;
    m_Map = nullptr;
    m_MovementPosition = 1;
}

MovementHandler::~MovementHandler()
{
}

bool MovementHandler::IsInColision(int64 p_PosX, int64 p_PosY) const
{
    if (m_Map == nullptr)
        return true;

    if (p_PosX + (SKIN_OFFSET_SIZE_X / 2) < 0 || p_PosY < 0)
        return true;

    if (p_PosX + m_SizeX - (SKIN_OFFSET_SIZE_X / 2) >= m_Map->GetSizeX() * TILE_SIZE || p_PosY >= m_Map->GetSizeY() * TILE_SIZE)
        return true;

    std::vector<Case*> l_CaseList;
    for (uint8 i = 0; i <= m_SizeX - SKIN_OFFSET_SIZE_X; i += TILE_SIZE)
        l_CaseList.push_back(m_Map->GetCase((uint32)p_PosX + i + (SKIN_OFFSET_SIZE_X / 2), (uint32)p_PosY));
    l_CaseList.push_back(m_Map->GetCase((uint32)p_PosX + m_SizeX - SKIN_OFFSET_SIZE_X + (SKIN_OFFSET_SIZE_X / 2), (uint32)p_PosY));

    for (Case* l_Case : l_CaseList)
    {
        if (l_Case == nullptr)
            return true;

        if (l_Case->IsBlocking())
            return true;
    }

    return false;
}


bool MovementHandler::CheckNextMovement(uint32 p_PosX, uint32 p_PosY)
{
    if (m_MovementStack.empty())
        return false;

    bool l_NextMovement = false;
    MovementAction l_MovementAction = m_MovementStack.front();
    if (!l_MovementAction.m_PositionOptions)
        l_NextMovement = true;
    else if (!IsInAttack())
    {
        switch (GetOrientation())
        {
        case Orientation::Down:
            if (l_MovementAction.m_Pos.y <= p_PosY)
                l_NextMovement = true;
            break;
        case Orientation::Left:
            if (l_MovementAction.m_Pos.x >= p_PosX)
                l_NextMovement = true;
            break;
        case Orientation::Right:
            if (l_MovementAction.m_Pos.x <= p_PosX)
                l_NextMovement = true;
            break;
        case Orientation::Up:
            if (l_MovementAction.m_Pos.y >= p_PosY)
                l_NextMovement = true;
            break;
        default:
            break;
        }
    }
    if (!l_NextMovement)
        return false;

    if (l_MovementAction.m_PositionOptions)
    {
        m_Pos.x = l_MovementAction.m_Pos.x;
        m_Pos.y = l_MovementAction.m_Pos.y;
    }
    m_MovementStack.pop();

    if (l_MovementAction.m_ActionType == eActionType::Go)
        StartMovement((Orientation)l_MovementAction.m_Orientation);
    else if (l_MovementAction.m_ActionType == eActionType::Stop)
        StopMovement();
    else if (l_MovementAction.m_ActionType == eActionType::Attack)
        StartAttack((Orientation)l_MovementAction.m_Orientation);
    else if (l_MovementAction.m_ActionType == eActionType::StopAttack)
        StopAttack();
    return true;
}

void MovementHandler::Update(sf::Time p_Diff)
{   
    CheckNextMovement(m_Pos.x, m_Pos.y);
    UpdateAnimationWalk(p_Diff);
    UpdateAnimationAttack(p_Diff);

    if (!m_InMovement)
        return;

    int64 l_PosX = m_Pos.x;
    int64 l_PosY = m_Pos.y;

    m_DiffTime += p_Diff.asMicroseconds();

    while (m_DiffTime > ((UPDATE_TIME_MOVEMENT / (STEP_SIZE * m_Speed)) * 1000)) ///< 1000 because microsecond
    {
        /// UPDATE POSITION
        switch (m_Orientation)
        {
        case Orientation::Down:
            l_PosY += (uint32)((STEP_SIZE / STEP_SIZE));
            break;
        case Orientation::Left:
            l_PosX -= (uint32)((STEP_SIZE / STEP_SIZE));
            break;
        case Orientation::Right:
            l_PosX += (uint32)((STEP_SIZE / STEP_SIZE));
            break;
        case Orientation::Up:
            l_PosY -= (uint32)((STEP_SIZE / STEP_SIZE));
            break;
        default:
            break;
        }
        m_DiffTime -= (uint64)((UPDATE_TIME_MOVEMENT / (STEP_SIZE * m_Speed)) * 1000);

        if (!IsInColision(l_PosX, l_PosY))
        {
            m_Pos.x = (uint32)l_PosX;
            m_Pos.y = (uint32)l_PosY;
            m_InColision = false;
        }
        else
        {
            StopMovement();
            m_InColision = true;
        }
    }
}

void MovementHandler::UpdateAnimationWalk(sf::Time p_Diff)
{
    if (!m_InMovement)
        return;

    m_DiffTimeAnim += p_Diff.asMicroseconds();

    while (m_DiffTimeAnim > ((UPDATE_TIME_MOVEMENT / m_Speed) * 1000)) ///< 1000 because microsecond
    {
        /// UPDATE ANIMATION
        if (m_AnimPeriodAsc)
        {
            if (m_MovementPosition >= 2)
            {
                m_AnimPeriodAsc = false;
                m_MovementPosition--;
            }
            else
                m_MovementPosition++;
        }
        else
        {
            if (m_MovementPosition <= 0)
            {
                m_AnimPeriodAsc = true;
                m_MovementPosition++;
            }
            else
                m_MovementPosition--;
        }
       /* if (m_MovementPosition <= 0)
            m_MovementPosition = 1;
        else if (m_MovementPosition == 1)
            m_MovementPosition = 2;
        else
            m_MovementPosition = 0;*/

        m_DiffTimeAnim = 0;
    }
}

void MovementHandler::UpdateAnimationAttack(sf::Time p_Diff)
{
    if (m_InMovement)
        return;

    if (!m_InAttack)
        return;

    m_DiffTimeAnimAttack += p_Diff.asMicroseconds();

    while (m_DiffTimeAnimAttack > (UPDATE_TIME_MOVEMENT * 1000)) ///< 1000 because microsecond
    {
        /// UPDATE ANIMATION
        m_MovementPosition++;

        if (m_MovementPosition >= MAX_MOVEMENT_POSITION)
        {
            m_MovementPosition = 0;
            m_DiffTimeAnimAttack = 0;
            if (m_StopAttack)
            {
                m_InAttack = false;
                m_MovementPosition = 1;
            }
            return;
        }
        else
        m_DiffTimeAnimAttack -= (uint64)(UPDATE_TIME_MOVEMENT * 1000);
    }
}

bool MovementHandler::IsInMovement() const
{
    return m_InMovement;
}

bool MovementHandler::IsInAttack() const
{
    return m_InAttack;
}

void MovementHandler::StartMovement(Orientation p_Orientation)
{
    m_InMovement = true;
    m_MovementPosition = 2;
    m_Orientation = p_Orientation;
}

void MovementHandler::StartMovement()
{
    m_InMovement = true;
    m_MovementPosition = 2;
}

void MovementHandler::StopMovement()
{
    m_InMovement = false;
    m_MovementPosition = 1;
    m_DiffTime = 0;
    m_DiffTimeAnim = 0;
}

void MovementHandler::StartAttack(Orientation p_Orientation)
{
    if (m_InAttack)
        return;
    m_Orientation = p_Orientation;
    StartAttack();
}

void MovementHandler::StartAttack()
{
    if (m_InAttack)
        return;
    StopMovement();
    m_MovementPosition = 0;
    m_InAttack = true;
    m_StopAttack = false;
    m_DiffTimeAnimAttack = 0;
}

void MovementHandler::StopAttack()
{
    m_StopAttack = true;
}

void MovementHandler::SetOrientation(Orientation p_Orientation)
{
    m_Orientation = p_Orientation;
}

void MovementHandler::SetMap(Map* p_Map)
{
    m_Map = p_Map;
}

float MovementHandler::GetSpeed() const
{
    return m_Speed;
}

Orientation MovementHandler::GetOrientation() const
{
    return m_Orientation;
}

uint8 MovementHandler::GetMovementPosition()
{
    if (!m_InMovement)
        return m_MovementPosition;

    return m_MovementPosition;
}

bool MovementHandler::GetInColision() const
{
    return m_InColision;
}

void MovementHandler::SetPosX(uint32 p_PosX)
{
    m_Pos.x = p_PosX;
}

void MovementHandler::SetPosY(uint32 p_PosY)
{
    m_Pos.y = p_PosY;
}

uint32 MovementHandler::GetPosX() const
{
    return m_Pos.x;
}

uint32 MovementHandler::GetPosY() const
{
    return m_Pos.y;
}

float MovementHandler::GetPosXAtIntant()
{
    switch (m_Orientation)
    {
    case Orientation::Right:
        return (float)m_Pos.x + ((float)(STEP_SIZE / STEP_SIZE) * ((float)m_DiffTime / ((float)UPDATE_TIME_MOVEMENT * (1000.0f / STEP_SIZE))));
        break;
    case Orientation::Left:
        return (float)m_Pos.x - ((float)(STEP_SIZE / STEP_SIZE) * ((float)m_DiffTime / ((float)UPDATE_TIME_MOVEMENT * (1000.0f / STEP_SIZE))));
        break;
    default:
        break;
    }

    return (float)m_Pos.x;
}

float MovementHandler::GetPosYAtIntant()
{
    switch (m_Orientation)
    {
    case Orientation::Down:
        return (float)m_Pos.y + ((float)(STEP_SIZE / STEP_SIZE) * ((float)m_DiffTime / ((float)UPDATE_TIME_MOVEMENT * (1000.0f / STEP_SIZE))));
        break;
    case Orientation::Up:
        return (float)m_Pos.y - ((float)(STEP_SIZE / STEP_SIZE) * ((float)m_DiffTime / ((float)UPDATE_TIME_MOVEMENT * (1000.0f / STEP_SIZE))));
        break;
    default:
        break;
    }

    return (float)m_Pos.y;
}

void MovementHandler::SetSpeed(float p_Speed)
{
    m_Speed = p_Speed;
}

void MovementHandler::AddMovementToStack(eActionType p_Action)
{
    MovementAction l_Act;
    l_Act.m_ActionType = p_Action;
    l_Act.m_PositionOptions = false;
    m_MovementStack.push(l_Act);
}

void MovementHandler::AddMovementToStack(eActionType p_Action, Position p_Pos, Orientation p_Orientation)
{
    MovementAction l_Act;
    l_Act.m_ActionType = p_Action;
    l_Act.m_PositionOptions = true;
    l_Act.m_Pos = p_Pos;
    l_Act.m_Orientation = p_Orientation;
    m_MovementStack.push(l_Act);
}

void MovementHandler::ClearMovementStack()
{
    while (!m_MovementStack.empty())
        m_MovementStack.pop();
}
