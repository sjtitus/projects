/*______________________________________________________________________________
 * Player 
 * A player exists on a board, has a position, and can move. 
 *______________________________________________________________________________
*/

#include "Player.hpp"
#include <boost/assert.hpp>
#include <chrono>
#include <iostream>
            
// Constructor 
Player::Player(const std::string &name, Board * const pBoard)
: _name(name), _pBoard(pBoard) 
{
    _id = _GenerateId();
}    
        
// generate unique player id
// use <name>::<the number of microseconds since the epoch>
std::string Player::_GenerateId()
{
    auto mse = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return std::string(_name) + "::" + std::to_string(mse);
}


// Movement
void Player::MoveUp(uint32_t step)
{
    // negative y with clamp to 0 (provided by Position) 
    _Position.move_y(step, true);
}

void Player::MoveDown(uint32_t step) 
{
    // positive y with clamp to height of board
    _Position.move_y(step, false);
    if (_Position.y() > _pBoard->hlimit())
        _Position.set_y(_pBoard->hlimit());
}

void Player::MoveLeft(uint32_t step) 
{
    // negative x with clamp to 0 (provided by Position) 
    _Position.move_x(step, true);
}

void Player::MoveRight(uint32_t step) 
{
    // positive x with clamp to width of board
    _Position.move_x(step, false);
    if (_Position.x() > _pBoard->wlimit())
        _Position.set_x(_pBoard->wlimit());
}

