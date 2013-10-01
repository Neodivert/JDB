/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

    This file is part of JDB.

    JDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JDB.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "tool.hpp"

namespace jdb {

/***
 * 1. Initialization
 ***/

Tool::Tool( const std::shared_ptr< m2g::AnimationData >& animationData ) :
    Animation( animationData )
{
}


/***
 * 2. Handlers
 ***/

void Tool::handleMouseButtonDown( Danger** dangers, unsigned int nDangers )
{
    unsigned int i = 0;

    while( ( i < nDangers ) &&
           !collide( *( dangers[i] ) ) ){
        i++;
    }

    if( i < nDangers ){
        dangers[i]->playerAction( PlayerAction::HAND_CLICK );
    }
}


} // namespace jdb
