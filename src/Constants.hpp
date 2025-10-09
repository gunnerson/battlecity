#pragma once

namespace constants {

inline constexpr int refreshRate{30};
inline constexpr int maxX{208}; // Battlefield size, horizontal
inline constexpr int maxY{208}; // Battlefield size, vertical
inline constexpr int ofX{4};    // Frame size, horizontal
inline constexpr int ofY{20};   // Frame size, vertical
inline constexpr int info{36};  // Info panel width

// Time in seconds tank is disabled after friendly fire hit
inline constexpr int friendlyFire{1};

// How many moves tank slides for on ice
inline constexpr int slide{8};

// Time in seconds upgrade is available before it disappears
inline constexpr int upgradeLifetime{15};

// "Shovel" upgrade duration, time in seconds
inline constexpr int upgradeShovel{15};

// "Helmet" upgrade duration, time in seconds
inline constexpr int upgradeHelmet{10};

// "Freeze" upgrade duration, time in seconds
inline constexpr int upgradeFreeze{10};

// Time in seconds - how often NPC can turn
inline constexpr int npcTurnFrequency{2};
//
// Time in seconds - how often NPCs can spawn
inline constexpr int spawnDelay{5};

// How often NPCs shoot, 1s / n
inline constexpr int npcShootFrequency{3};
} // namespace constants
