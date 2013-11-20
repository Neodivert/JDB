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

#ifndef DANGER_HPP
#define DANGER_HPP

#include "player_action.hpp"
#include "danger_data.hpp"
#include "../dependencies/m2g/src/drawables/animation.hpp"

namespace jdb {

class Danger : public m2g::Animation
{
    public:
        float hp;
        unsigned int state;

        DangerDataPtr dangerData;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Danger( DangerDataPtr dangerData_ );


        /***
         * 2. Getters
         ***/
        float getDamage() const ;
        DangerDataPtr getDangerData() const ;


        /***
         * 3. Setters
         ***/
        // TODO: Overload Animation setters.
        void setDangerData( DangerDataPtr dangerData_ );
        void setState( int newState );


        /***
         * 4. Updating
         ***/
        bool playerAction( PlayerAction playerAction );
        void reset();
};

} // namespace jdb

#endif // DANGER_HPP
