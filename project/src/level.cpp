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

#include "level.hpp"
#include "dependencies/graphics/src/dependencies/tinyxml2/tinyxml2.h"

namespace jdb {

const unsigned int N_SANDWICHES = 5;
const float SANDWICHES_END_POINT = -300.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;
const unsigned int N_DANGERS = N_SANDWICHES * 3;
const unsigned int FPS = 25;
const unsigned int REFRESH_TIME = 1000 / FPS;


Level::Level( SDL_Window* window_, SDL_Surface* screen_, unsigned int screenWidth, unsigned int screenHeight )
    : window( window_ ),
      screen( screen_ )
{
    projectionMatrix = glm::ortho( 0.0f, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f, -1.0f );
}


void Level::runSurvivalLevel( unsigned int index )
{
    tinyxml2::XMLNode* levelNode = nullptr;
    tinyxml2::XMLElement* levelElement = nullptr;

    unsigned int i = 0;
    float initialSpeed;
    float speedStep;
    unsigned int timeLapse;

    // Open the levels configuration file.
    xmlFile.LoadFile( "data/config/levels.xml" );

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "survival_levels" )->FirstChildElement( "survival_level" ) );
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "survival_level" );
        i++;
    }

    // If the index XML node doesn't exist, throw an exception.
    if( i < index ){
        throw std::runtime_error( "ERROR: Survival level not found" );
    }

    // Load the sandwiches data.
    loadSandwichData();

    // Load the dangers data.
    loadDangerData();

    // Get the level parameters.
    levelElement = (tinyxml2::XMLElement*)levelNode->FirstChildElement( "speed" );
    initialSpeed = levelElement->FloatAttribute( "initial" );
    speedStep = levelElement->FloatAttribute( "step" );
    timeLapse = (unsigned int)( levelElement->IntAttribute( "time_lapse" ) );

    // Execute the main loop.
    survivalLoop( initialSpeed, speedStep, timeLapse );
}


void Level::survivalLoop( float initialSpeed, float speedStep, unsigned int timeLapse )
{
    unsigned int i;
    unsigned int firstSandwich = 0;
    unsigned int lastSandwich = N_SANDWICHES - 1;
    std::mutex speedMutex;
    float speed = initialSpeed;
    unsigned int nDraws = 0;
    float jacobHp = 100.0f;
    char character[2] = { ' ', '\0' };
    unsigned int seconds, minutes;
    char buffer[16];

    // Initialize the text renderer.
    m2g::TextRenderer textRenderer;
    std::cout << "loadFont: " << textRenderer.loadFont( "data/fonts/LiberationSans-Bold.ttf", 50 ) << std::endl;

    try
    {
        SDL_Event event;
        bool quit = false;

        quit = true;
        while( quit ){
            textRenderer.drawText( projectionMatrix, "PRESS ANY KEY TO START", 0, 0, 0 );
            SDL_GL_SwapWindow( window );
            SDL_WaitEvent( &event );
            quit = ( event.type != SDL_KEYDOWN );
        }

        // Variable for time management.
        Uint32 t0 = 0;
        Uint32 t1 = 0;

        // Create the graphic Library and the sprite pointers.
        Sandwich* sandwiches[N_SANDWICHES];
        Tool* tool = nullptr;
        //m2g::Sprite* conveyorBelt = nullptr;

        // Make the cursor invisible.
        SDL_ShowCursor( SDL_DISABLE );

        // Load the sandwiches' sprites and move them to their positions.
        for( i=0; i < N_SANDWICHES; i++ ){
            sandwiches[i] = new Sandwich( sandwichData[0], &dangerData );

            sandwiches[i]->moveTo( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );

            sandwiches[i]->populate( dangerData );
        }

        /*
        // Load the rest of the sprites and animations.
        */
        std::vector< std::shared_ptr< m2g::AnimationData > > toolsData;
        graphicsLoader.loadAnimationsData( toolsData, "data/img/tools" );

        tool = new Tool( toolsData[0] );
        /*
        conveyorBelt = new m2g::Sprite( library.getTileset( "conveyor_belt.png" ) );

        // Set the conveyor belt's sprite at its final position.
        conveyorBelt->translate( 0, 256 );
        */

        // Start the timer and set its callback function.
        timer.init( timeLapse, [&](){
            speedMutex.lock();
            speed += speedStep;
            speedMutex.unlock();

            coutMutex.lock();
            std::cout << "New speed! (" << speed << ")" << std::endl;
            coutMutex.unlock();
        });

        // Keep rendering until player tell us to stop.
        while( !quit ){
            glClear ( GL_COLOR_BUFFER_BIT );
            t0 = SDL_GetTicks();
            while( (t1 - t0) < REFRESH_TIME ){
                if( SDL_PollEvent( &event ) != 0 ){
                    switch( event.type ){
                        case SDL_QUIT:
                            quit = true;
                        break;
                        case SDL_MOUSEBUTTONDOWN:
                            tool->handleMouseButtonDown( sandwiches, N_SANDWICHES );
                        break;
                        case SDL_KEYDOWN:
                            switch( event.key.keysym.sym ){
                                case SDLK_ESCAPE:
                                    quit = true;
                                break;
                            }
                        break;
                        case SDL_MOUSEMOTION:
                            tool->moveTo( event.motion.x, event.motion.y );
                        break;
                    }
                }
                t1 = SDL_GetTicks();
            }
            t0 = SDL_GetTicks();
            t1 = SDL_GetTicks();

            // Clear screen.
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            // Bind the tileset's buffer.
            m2g::Tileset::bindBuffer();

            // Draw the conveyor belt.
            //conveyorBelt->draw( projectionMatrix );

            // Draw the sandwiches
            for( i=0; i < N_SANDWICHES; i++ ){
                sandwiches[i]->draw( projectionMatrix );
            }

            // Move the sandwiches
            speedMutex.lock();
            for( i=0; i < N_SANDWICHES; i++ ){
                sandwiches[i]->translate( -speed, 0.0f );
            }
            speedMutex.unlock();

            // Draw the tool.
            tool->draw( projectionMatrix );

            // Draw text.
            //textRenderer.drawText( projectionMatrix, "MOISES", 0, 0, 0 );
            //textRenderer.drawText( projectionMatrix, character, 0, 700, 0 );

            // Show Jacob's life.
            sprintf( buffer, "%d", (int)jacobHp );
            textRenderer.drawText( projectionMatrix, buffer, 0, 0, 0 );

            // Show the time.
            seconds = timer.getSeconds();
            minutes = seconds / 60;
            seconds = seconds % 60;
            sprintf( buffer, "TIME: %02d:%02d", minutes, seconds );
            textRenderer.drawText( projectionMatrix, buffer, 0, 250, 0 );

            // Check if the first sandwich reached the sandwiches end point and, in that case,
            // translate it and is dangers behind the last sandwich.
            if( sandwiches[firstSandwich]->getX() < SANDWICHES_END_POINT ){

                // Hurt Jacob! (muahahaha!)
                jacobHp -= sandwiches[firstSandwich]->getDamage();
                coutMutex.lock();
                std::cout << "jacob's hp: " << jacobHp << std::endl;
                coutMutex.unlock();

                // Populate new sandwich.
                sandwiches[firstSandwich]->populate( dangerData );

                // Sandwich translation.
                sandwiches[firstSandwich]->translate(
                            sandwiches[lastSandwich]->getX()
                            - sandwiches[firstSandwich]->getX()
                            + DISTANCE_BETWEEN_SANDWICHES,
                            0.0f );

                // Change the indices for the first and last sandwiches.
                firstSandwich = (firstSandwich + 1) % N_SANDWICHES;
                lastSandwich = (lastSandwich + 1) % N_SANDWICHES;
            }

            // Refresh screen.
            SDL_GL_SwapWindow( window );

            nDraws++;
            if( nDraws >= FPS ){
                nDraws = 0;
                character[0]++;
            }
        }


        // Free resources
        delete tool;
        //delete conveyorBelt;
        for( i=0; i < N_SANDWICHES; i++ ){
            delete sandwiches[i];
        }

        // Stop the timer.
        timer.stop();

    }catch( std::runtime_error& e ){
        std::cerr << e.what() << std::endl;
    }
}


/***
 * 2. Loading
 ***/

void Level::loadSandwichData()
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement* sandwichXMLElement = nullptr;

    // Load the sandwiches data.
    //graphicsLoader.loadTilesets( sandwichesData, "data/img/sandwiches" );


    // Load the dangers data.
    document.LoadFile( "./data/config/sandwiches.xml" );
    sandwichXMLElement = ( document.RootElement() )->FirstChildElement( "sandwich" );
    while( sandwichXMLElement ){
        sandwichData.emplace_back( new SandwichData( sandwichXMLElement ) );

        sandwichXMLElement = sandwichXMLElement->NextSiblingElement();
    }
}


void Level::loadDangerData()
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement* dangerXMLElement = nullptr;

    // Load the dangers data.
    document.LoadFile( "./data/config/dangers.xml" );
    dangerXMLElement = ( document.RootElement() )->FirstChildElement( "danger" );
    while( dangerXMLElement ){
        dangerData.emplace_back( new DangerData( dangerXMLElement ) );

        dangerXMLElement = dangerXMLElement->NextSiblingElement();
    }
}


void Level::drawTimer( int time )
{
    std::cout << "seconds: " << time << std::endl;
}


} // namespace jdb
