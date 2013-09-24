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

#include "library.hpp"

namespace m2g {

/***
 * 1. Initialization and destruction
 ***/

Library::Library()
{}


/***
 * 2. File management
 ***/

void Library::loadFile( const std::string& filePath )
{
    tinyxml2::XMLDocument xmlFile;
    tinyxml2::XMLNode* tilesetNode;
    std::string imagesFolder;

    // Get the path to the "default" images folder.
    // TODO: Allow other skins loading.
    imagesFolder = filePath.substr( 0, filePath.rfind( "/" ) ) + "/default/";

    // Open the requested file.
    xmlFile.LoadFile( filePath.c_str() );

    // Load the first tileset XML node.
    tilesetNode = ( xmlFile.FirstChildElement( "library" ) )->FirstChildElement();

    // Keep reading tilesets until the end of file.
    while( tilesetNode ){
        if( !strcmp( tilesetNode->Value(), "tileset" ) ){
            // Load the tileset in the current XML element.
            tilesets.push_back( std::shared_ptr< Tileset >( new Tileset( tilesetNode, imagesFolder.c_str() ) ) );
        }else{
            std::cout << "Pushing back [" << tilesetNode->Value() << "] ..." << std::endl;
            animationsData.push_back( std::shared_ptr< AnimationData >( new AnimationData( tilesetNode, imagesFolder.c_str() ) ) );
            std::cout << "Pushing back [" << tilesetNode->Value() << "] ...OK" << std::endl;
        }

        // Get next XML element.
        tilesetNode = tilesetNode->NextSiblingElement();
    }
}


/***
 * 3. Setters and getters
 ***/

const std::shared_ptr< m2g::Tileset > Library::getTileset( const unsigned int& index ) const
{
    return tilesets[index];
}


const std::shared_ptr< m2g::Tileset> Library::getTileset( const std::string& imageName ) const
{
    unsigned int i = 0;

    while( ( i < tilesets.size() ) &&
           ( tilesets[i]->getName() != imageName) ){
        i++;
    }

    if( i >= tilesets.size() ){
        throw std::runtime_error( "ERROR: Tileset [" + imageName + "] not found in library" );
    }

    return tilesets[i];
}



} // namespace m2g
