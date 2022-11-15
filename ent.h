#include <windows.h>
#include <cstdio>
#include <cstdint>
struct Vector3 { float x, y, z; };

class localent
{
public:
    char pad_0000[4]; //0x0000
    Vector3 headpos; //0x0004
    Vector3 velocity; //0x0010
    char pad_001C[24]; //0x001C
    Vector3 xyz; //0x0034
    Vector3 angles; //0x0040
    char pad_004C[172]; //0x004C
    int32_t hp; //0x00F8
    char pad_00FC[248]; //0x00FC
    int32_t DEATHS; //0x01F4
    char pad_01F8[4]; //0x01F8
    int32_t KILLS; //0x01FC
    char pad_0200[36]; //0x0200
    int8_t bAttack; //0x0224
    char pad_0225[263]; //0x0225
    int32_t team; //0x032C
    char pad_0330[68]; //0x0330
    class weapon* currWeapon; //0x0374
    char pad_0378[216]; //0x0378
}; //Size: 0x0450
static_assert(sizeof(localent) == 0x450);

class weapon
{
public:
    char pad_0000[4]; //0x0000
    int32_t weaponid; //0x0004
    class localent* owner; //0x0008
    char pad_000C[4]; //0x000C
    class ammoPtr* ammoReserve; //0x0010
    class ammoPtr* ammoMag; //0x0014
    char pad_0018[44]; //0x0018
}; //Size: 0x0044
static_assert(sizeof(weapon) == 0x44);

class ammoPtr
{
public:
    int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof(ammoPtr) == 0x4);

class entlistclass
{
public:
    class entlist* entlistptr; //0x0000
}; //Size: 0x0004
static_assert(sizeof(entlistclass) == 0x4);

class entlist
{
public:
    class localent* ents[32]; //0x0000
}; //Size: 0x0080
static_assert(sizeof(entlist) == 0x80);