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

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "../utilities/dependencies.hpp"

namespace jdb {

class Drawable
{
    protected:
        glm::mat4 transformationMatrix;
        glm::vec4 position;

    public:
        /***
         * 1. Initialization
         ***/
        Drawable();

        /***
         * 2. Getters and setters
         ***/
        GLfloat getX() const;

        /***
         * 3. Transformations
         ***/
        virtual void translate( const float& tx, const float& ty );


        /***
         * 4. Drawing
         ***/
        virtual void draw( const glm::mat4& projectionMatrix ) const = 0;
};


} // Namespace jdb

#endif // DRAWABLE_HPP
