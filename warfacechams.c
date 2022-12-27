#include <iostream>
#include <unordered_map>

// Warface includes and namespaces
#include "Entity.h"

using namespace std;

// Offsets for the player entity and the canSee method
const uintptr_t PLAYER_ENTITY_OFFSET = 0x1234;
const uintptr_t CAN_SEE_METHOD_OFFSET = 0x5678;

int main()
{
    // Get the base address of the game's process
    uintptr_t baseAddress = getGameBaseAddress();

    // Get the player's entity using the offset
    Entity* player = *(Entity**)(baseAddress + PLAYER_ENTITY_OFFSET);

    // Create a map to store the enemies that we have already highlighted
    unordered_map<Entity*, bool> highlightedEnemies;

    // Main game loop
    while (gameIsRunning())
    {
        // Get a list of all enemies in the game
        vector<Entity*> enemies = getEnemies(baseAddress);

        // Iterate through the enemies
        for (Entity* enemy : enemies)
        {
            // Check if we have already highlighted this enemy
            if (highlightedEnemies.count(enemy) > 0)
            {
                continue;
            }

            // Check if the enemy is within sight of the player using the canSee method and the offset
            bool (*canSee)(Entity*, Entity*) = (bool (*)(Entity*, Entity*))(baseAddress + CAN_SEE_METHOD_OFFSET);
            if (canSee(player, enemy))
            {
                // Highlight the enemy in red
                enemy->setColor(255, 0, 0);

                // Mark the enemy as highlighted
                highlightedEnemies[enemy] = true;
            }
        }

        // Sleep for a short time to reduce CPU usage
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}
