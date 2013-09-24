/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of M2G.
 *
 * M2G is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * M2G is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2G.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "sprite.hpp"
#include <iostream>

namespace m2g {

GLint Sprite::mvpMatrixLocation = -1;
GLint Sprite::samplerLocation = -1;
GLint Sprite::sliceLocation = -1;

/***
 * 1. Initialization
 ***/

Sprite::Sprite()
{
    GLint currentProgram;

    // If we still don't have the locations of the shader uniforms, we
    // search them here.
    if( mvpMatrixLocation == -1 ){
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );

        mvpMatrixLocation = glGetUniformLocation( currentProgram, "mvpMatrix" );
        samplerLocation = glGetUniformLocation( currentProgram, "tex" );
        sliceLocation = glGetUniformLocation( currentProgram, "slice" );

        std::cout << "mvpMatrixLocation: " << mvpMatrixLocation << std::endl
                  << "samplerLocation: " << samplerLocation << std::endl
                  << "sliceLocation: " << sliceLocation << std::endl;

        currentTile = 0;
    }

    // Connect sampler to texture unit 0.
    glUniform1i( samplerLocation, 0 );
    std::cout << "Location of shader variable \"sampler\" set : " << gluErrorString( glGetError() ) << std::endl;
}


/***
 * 2. Getters and setters
 ***/

void Sprite::setTileset( std::shared_ptr< Tileset > tileset )
{
    // Start sharing the new tileset.
    this->tileset = tileset;
}


const std::shared_ptr< Tileset > Sprite::getTileset()
{
    return tileset;
}


/***
 * 3. Current tile management
 ***/

void Sprite::nextTile()
{
    if( currentTile < (tileset->nTiles - 1) ){
        currentTile++;
    }else{
        currentTile = 0;
    }
}

void Sprite::previousTile()
{
    if( currentTile ){
        currentTile--;
    }else{
        currentTile = tileset->nTiles - 1;
    }
}

void Sprite::setTile( const GLuint tile )
{
    if( tile < tileset->nTiles ){
        currentTile = tile;
    }else{
        throw std::runtime_error( "ERROR: tile index out of limits" );
    }
}


GLuint Sprite::getCurrentTile() const
{
    return currentTile;
}


/***
 * 4. Collision detection
 ***/

const std::vector<Rect>* Sprite::getCollisionRects() const
{
    return &( tileset->collisionRects[currentTile] );
}


/***
 * 5. Drawing
 ***/

void Sprite::draw( const glm::mat4& projectionMatrix ) const {
    // Load the sprite's attributes for rendering.
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D_ARRAY, tileset->texture );

    // Send current tile index to shader.
    glUniform1ui( sliceLocation, currentTile );

    // Send MVP matrix to shader.
    sendMVPMatrixToShader( projectionMatrix * glm::translate( glm::mat4( 1.0f ), glm::vec3( position.x, position.y, 0.0f ) ) );

    // Draw the sprite.
    tileset->tilesetsBuffer->draw( tileset->bufferIndex );
    //glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}


/***
 * 6. Auxiliar methods
 ***/

void Sprite::sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const
{
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0] );
}


} // Namespace m2g
