// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <d3d9.h>
#include <tchar.h>
#include <consoleapi.h>
#include "ent.h"

bool TriggerBot = false;
bool SuperSpeed = false;
bool SuperAR = false;
bool instaReload = false;
bool infAmmo = false;
bool infHp = false;
bool noRecoil = false;
bool teleporterSet = false;
bool superJump = false;


typedef int(__cdecl* _getEntCrosshair)();
typedef void(__cdecl* _speedFunc)();


void printInfo();


DWORD WINAPI HackThread(HMODULE hModule) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

    localent* localPlayer = *(localent**)(moduleBase + 0x10F4F4);

    _getEntCrosshair getEntCrosshair = (_getEntCrosshair)(moduleBase + 0x607C0);
    _speedFunc speedFunc = (_speedFunc)(moduleBase + 0x25770); 

    Vector3 tpPos = { 0, 0, 0 };
    printInfo();

  
    while (1) {
        if (localPlayer) {
            if (GetAsyncKeyState(VK_F2) & 1) {
                TriggerBot = !TriggerBot;
                printInfo();
            }
            if (GetAsyncKeyState(VK_F3) & 1) {
                SuperSpeed = !SuperSpeed;
                printInfo();
            }
            if (GetAsyncKeyState(VK_F4) & 1) {
                localPlayer->KILLS += 888;
                printInfo();
            }

            if (GetAsyncKeyState(VK_F5) & 1) {
                SuperAR = !SuperAR;
                if (SuperAR) {
                    Nop((BYTE*)(moduleBase + 0x637E4), 2);
                }
                else {
                    Patch((BYTE*)(moduleBase + 0x637E4), (BYTE*)"\x89\x0a", 2);
                }
                printInfo();
            }
            if (GetAsyncKeyState(VK_F6) & 1) {
                instaReload = !instaReload;
                if (instaReload) {
                    Nop((BYTE*)(moduleBase + 0x61E20), 2);
                }
                printInfo();
            }
            if (GetAsyncKeyState(VK_F7) & 1) {
                infAmmo = !infAmmo;
                if (infAmmo) {
                    Patch((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xff\x06", 2);
                }
                else {
                    Patch((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xff\x0e", 2);
                }
                printInfo();
            }
            if (GetAsyncKeyState(VK_F8) & 1) {
                infHp = !infHp;
                printInfo();
            }
            if (GetAsyncKeyState(VK_F9) & 1) {
                noRecoil = !noRecoil;
                if (noRecoil) {
                    Nop((BYTE*)(moduleBase + 0x622AC), 3);
                    Nop((BYTE*)(moduleBase + 0x621E4), 3);
                }
                else {
                    Patch((BYTE*)(moduleBase + 0x622AC), (BYTE*)"\xd9\x5a\x4c", 3);
                    Patch((BYTE*)(moduleBase + 0x621E4), (BYTE*)"\xd9\x5a\x4c", 3);
                }
                printInfo();
            }
            if (GetAsyncKeyState(VK_F10) & 1) {
                // store location
                teleporterSet = !teleporterSet;
                tpPos = localPlayer->xyz;
                printInfo();
            }
            if (GetAsyncKeyState(VK_F12) & 1) {
                superJump = !superJump;
                printInfo();
            }

            if (TriggerBot) {
                localent* crossHairEnt = (localent*)getEntCrosshair();
                if (crossHairEnt) {
                    if (localPlayer->team != crossHairEnt->team) {
                        localPlayer->bAttack = 1;
                    }
                }
                else {
                    localPlayer->bAttack = 0;
                }
            }

            if (SuperSpeed) {
                speedFunc();
            }
            if (infHp) {
                localPlayer->hp = 1337;
            }
            if (superJump) {
                if (GetAsyncKeyState(VK_SPACE) & 1) {
                    localPlayer->xyz.z += 5;
                }
            }
            if (teleporterSet) {
                if (GetAsyncKeyState(VK_F11) & 1) {
                    localPlayer->xyz = tpPos;
                }
            }
        }
        Sleep(5);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;

}

void printInfo() {

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "|    cluckwork's insane AC trainer    |" << std::endl;
    std::cout << "|-------------------------------------|" << std::endl;
    std::cout << "|                                     |" << std::endl;
    std::cout << "| [F2] TriggerBot                 >" << TriggerBot << "  |" << std::endl;
    std::cout << "| [F3] Super Speed                >" << SuperSpeed << "  |" << std::endl;
    std::cout << "| [F4] Give yourself 888 kills        |" << std::endl;
    std::cout << "| [F5] Super Assault Rifle        >" << SuperAR << "  |" << std::endl;
    std::cout << "| [F6] Instant Reload             >" << instaReload << "  |" << std::endl;
    std::cout << "| [F7] Infinite Ammo              >" << infAmmo << "  |" << std::endl;
    std::cout << "| [F8] Infinite HP                >" << infHp << "  |" << std::endl;
    std::cout << "| [F9] No Recoil                  >" << noRecoil << "  |" << std::endl;
    std::cout << "| [F10] Set teleporter to current pos |" << std::endl;
    std::cout << "| [F11] Teleport to pos               |" << std::endl;
    std::cout << "| [F12] Low Gravity               >" << superJump << "  |" << std::endl;
    std::cout << "|                                     |" << std::endl;
    std::cout << "|-------------------------------------|" << std::endl;

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

