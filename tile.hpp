#ifndef     __HPP_TILE__
#define     __HPP_TILE__

enum TileStatus {
    WATER,      // No ship or attempted strike present
    SHIP,       // Ship present
    SPLASH,     // Attempted strike (no ship present)
    FIRE       // Successful strike (ship present)
};

struct Tile {
    Tile();
    Tile(TileStatus status);
    TileStatus status;
};

#endif