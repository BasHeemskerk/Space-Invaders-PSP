#include "included/sprite.hpp"

namespace sprite{
    namespace playerSprite{
        Image *playerSprite;

        void loadPlayerSprite(){
            playerSprite = loadPng("romfs/gfx/player/player.png");
        }
        void unloadPlayerSprite(){
            freeImage(playerSprite);
        }
    }
    namespace enemySprite{

    }
    namespace bulletSprite{

    }
}