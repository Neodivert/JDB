/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

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

#include "main_menu.hpp"
#include "../levels/campaign_level.hpp"
#include "../levels/survival_level.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>

namespace jdb {

/***
 * 1. Creation
 ****/

MainMenu::MainMenu( sf::RenderWindow& window, SoundManager* soundManager ) :
    GameState( window ),
    soundManager_( *soundManager ),
    gui_( window )
{}


/***
 * 3. GameState interface
 ***/

void MainMenu::init()
{
    gui_.setGlobalFont( "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf" );

    const std::vector< std::string > buttonTexts =
    {
        "Play campaign",
        "Play survival",
        "Exit game"
    };
    const std::vector< std::function<void()> > callbacks =
    {
        [this](){
            std::unique_ptr< Level > level = std::unique_ptr< Level >(
                       new CampaignLevel( window_, &soundManager_, 0 ) );
            switchState( *level );
        },
        [this](){
            std::unique_ptr< Level > level = std::unique_ptr< Level >(
                       new SurvivalLevel( window_, &soundManager_, 0 ) );
            switchState( *level );
        },
        [this](){
            requestStateExit();
        }
    };

    // Buttons' layout magnitudes.
    const sf::Vector2f BUTTON_SIZE = { 260, 80 };
    const float BUTTON_OFFSET = 25;
    const float BUTTONS_GROUP_HEIGHT =
            BUTTON_SIZE.y * buttonTexts.size() + BUTTON_OFFSET * (buttonTexts.size() - 1);
    sf::Vector2f buttonPos = {
        (window_.getSize().x - BUTTON_SIZE.x) / 2,
        (window_.getSize().y - BUTTONS_GROUP_HEIGHT) / 2,
    };

    // Create buttons.
    unsigned int buttonCallbackId = 0;
    for( const std::string& buttonText : buttonTexts ){
        tgui::Button::Ptr button = tgui::Button::create();
        button->setSize( BUTTON_SIZE.x, BUTTON_SIZE.y );
        button->setPosition( buttonPos.x, buttonPos.y );
        button->setText( buttonText );

        buttonPos.y += BUTTON_SIZE.y + BUTTON_OFFSET;

        button->connect( "pressed", callbacks[buttonCallbackId] );
        buttonCallbackId++;

        gui_.add( button );
    }
}


void MainMenu::handleEvents()
{
    sf::Event event;
    std::unique_ptr< Level > level = nullptr;

    if( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            requestStateExit();
        }else if( event.type == sf::Event::KeyPressed ){
            if( event.key.code == sf::Keyboard::Escape ){
                requestStateExit();
            }
        }else{
            gui_.handleEvent( event );
        }
    }
}


void MainMenu::update( unsigned int ms )
{
    (void)( ms );
}


void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    (void)( target );
    (void)( states );
    window_.clear( sf::Color( 0xDC, 0xF1, 0xF1, 0xFF ) );

    gui_.draw();
}


void MainMenu::pause()
{
}

void MainMenu::resume()
{
}

} // namespace jdb
