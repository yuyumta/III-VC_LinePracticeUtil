#include "plugin.h"
#include "CHud.h"
#include "CTimer.h"
#include "CCamera.h"
#include "extensions/ScriptCommands.h"
using namespace plugin;

class IIILinePracticeUtil {
public:
    IIILinePracticeUtil() {

        static CVector vposition, pposition;
        static CVector vspeed, pspeed;
        static CVehicle* lastVehicle;
        static float ux, uy, uz, rx, ry, rz, ch, cv;
        static float vhealth = 10000.0f;
        static float phealth = 100.0f;
        static int wheelStatus_0, wheelStatus_1, wheelStatus_2, wheelStatus_3 = 0;
        static int doorStatus_0, doorStatus_1, doorStatus_2, doorStatus_3, doorStatus_4, doorStatus_5 = 0;
        static int engineStatus = 0;
        static int m_nLastSpawnedTime = 0;
        static wchar_t weird_string[1024];

        Events::processScriptsEvent += [] {

            CVehicle* vehicle = FindPlayerVehicle();
            CPlayerPed* player = FindPlayerPed();
            CPhysical* entity = (CPhysical*)FindPlayerEntity();
            CAutomobile* automobile = reinterpret_cast<CAutomobile*>(vehicle);

            if (KeyPressed(0x31) && (CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000)) && (player->m_bInVehicle)) {

                if (!vposition.x) {

                    lastVehicle = vehicle;

                    vposition = entity->GetPosition();
                    vspeed = vehicle->m_vecMoveSpeed;

                    ux = vehicle->m_matrix.up.x;
                    uy = vehicle->m_matrix.up.y;
                    uz = vehicle->m_matrix.up.z;
                    rx = vehicle->m_matrix.right.x;
                    ry = vehicle->m_matrix.right.y;
                    rz = vehicle->m_matrix.right.z;

                }

                if (player) {
                    player->m_fHealth = phealth;
                }

                if (vehicle) {
                    vehicle->m_fHealth = vhealth;
                }

                // head empty, no work
                /*
                //vehicle->m_nVehicleFlags.bIsDamaged = false;
                automobile->m_carDamage.SetWheelStatus(eWheels::WHEEL_FRONT_LEFT, wheelStatus_0);
                automobile->m_carDamage.SetWheelStatus(eWheels::WHEEL_REAR_LEFT, wheelStatus_1);
                automobile->m_carDamage.SetWheelStatus(eWheels::WHEEL_FRONT_RIGHT, wheelStatus_2);
                automobile->m_carDamage.SetWheelStatus(eWheels::WHEEL_REAR_RIGHT, wheelStatus_3);
                automobile->m_carDamage.SetDoorStatus(eDoors::BONNET, doorStatus_0);
                automobile->m_carDamage.SetDoorStatus(eDoors::BOOT, doorStatus_1);
                automobile->m_carDamage.SetDoorStatus(eDoors::DOOR_FRONT_LEFT, doorStatus_2);
                //automobile->OpenDoor(15, eDoors::DOOR_FRONT_LEFT, 0.1f);
                //automobile->SetDoorDamage(0, eDoors::DOOR_FRONT_LEFT, true);
                automobile->m_carDamage.SetDoorStatus(eDoors::DOOR_FRONT_RIGHT, doorStatus_3);
                automobile->m_carDamage.SetDoorStatus(eDoors::DOOR_REAR_LEFT, doorStatus_4);
                automobile->m_carDamage.SetDoorStatus(eDoors::DOOR_REAR_RIGHT, doorStatus_5);
                */
                automobile->m_carDamage.SetEngineStatus(engineStatus);


                // wtf does this do
                /* 
                int component;

                for (component = CAR_BUMP_FRONT; component < eCarNodes::CAR_NUM_NODES; component++) {
                    if (automobile->m_aCarNodes[component]) {
                        CMatrix mat(RwFrameGetMatrix(automobile->m_aCarNodes[component]));
                        mat.SetTranslate(mat.pos.x, mat.pos.y, mat.pos.z);
                        mat.UpdateRW();
                    }
                }
                */

                entity->Teleport(vposition);
                vehicle->m_vecMoveSpeed = vspeed;

                vehicle->m_matrix.up.x = ux;
                vehicle->m_matrix.up.y = uy;
                vehicle->m_matrix.up.z = uz;
                vehicle->m_matrix.right.x = rx;
                vehicle->m_matrix.right.y = ry;
                vehicle->m_matrix.right.z = rz;

                Command<eScriptCommands::COMMAND_RESTORE_CAMERA_JUMPCUT>();

                m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;
                std::wcsncpy(weird_string, L"Loaded", 1024);
                CHud::SetHelpMessage(weird_string, true);

            }

            if (KeyPressed(0x31) && (CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000)) && (!player->m_bInVehicle) && (Command<eScriptCommands::COMMAND_DOES_OBJECT_EXIST>(lastVehicle))) {

                Command<eScriptCommands::COMMAND_WARP_CHAR_INTO_CAR>(player, lastVehicle);

            }

            if (KeyPressed(0x32) && (CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000)) && (!player->m_bInVehicle)) {

                if (!pposition.x) {
                    pposition = entity->GetPosition();
                    pspeed = player->m_vecMoveSpeed;
                }

                if (player) {
                    player->m_fHealth = phealth;
                }

                if (vehicle) {
                    vehicle->m_fHealth = vhealth;
                }

                entity->Teleport(pposition);
                player->m_vecMoveSpeed = pspeed;
                player->ApplyMoveSpeed();

                TheCamera.m_asCams->m_fHorizontalAngle = ch;
                TheCamera.m_asCams->m_fVerticalAngle = cv;

                m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;
                std::wcsncpy(weird_string, L"Loaded", 1024);
                CHud::SetHelpMessage(weird_string, true);

            }

            if (KeyPressed(VK_TAB) && (CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000))) {

                if (vehicle) {
                    vhealth = vehicle->m_fHealth;
                }

                if (player) {
                    phealth = player->m_fHealth;
                }

                if (!player->m_bInVehicle) {

                    pposition = entity->GetPosition();
                    pspeed = player->m_vecMoveSpeed;

                    ch = TheCamera.m_asCams->m_fHorizontalAngle;
                    cv = TheCamera.m_asCams->m_fVerticalAngle;

                }
                else {

                    lastVehicle = vehicle;
                    
                    /*
                    wheelStatus_0 = automobile->m_carDamage.GetWheelStatus(eWheels::WHEEL_FRONT_LEFT);
                    wheelStatus_1 = automobile->m_carDamage.GetWheelStatus(eWheels::WHEEL_REAR_LEFT);
                    wheelStatus_2 = automobile->m_carDamage.GetWheelStatus(eWheels::WHEEL_FRONT_RIGHT);
                    wheelStatus_3 = automobile->m_carDamage.GetWheelStatus(eWheels::WHEEL_REAR_RIGHT);
                    doorStatus_0 = automobile->m_carDamage.GetDoorStatus(eDoors::BONNET);
                    doorStatus_1 = automobile->m_carDamage.GetDoorStatus(eDoors::BOOT);
                    doorStatus_2 = automobile->m_carDamage.GetDoorStatus(eDoors::DOOR_FRONT_LEFT);
                    doorStatus_3 = automobile->m_carDamage.GetDoorStatus(eDoors::DOOR_FRONT_RIGHT);
                    doorStatus_4 = automobile->m_carDamage.GetDoorStatus(eDoors::DOOR_REAR_LEFT);
                    doorStatus_5 = automobile->m_carDamage.GetDoorStatus(eDoors::DOOR_REAR_RIGHT);
                    */
                    engineStatus = automobile->m_carDamage.GetEngineStatus();

                    vposition = entity->GetPosition();
                    vspeed = vehicle->m_vecMoveSpeed;

                    ux = vehicle->m_matrix.up.x;
                    uy = vehicle->m_matrix.up.y;
                    uz = vehicle->m_matrix.up.z;
                    rx = vehicle->m_matrix.right.x;
                    ry = vehicle->m_matrix.right.y;
                    rz = vehicle->m_matrix.right.z;

                }

                m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;
                std::wcsncpy(weird_string, L"Set", 1024);
                CHud::SetHelpMessage(weird_string, true);

            }

            /*
            if (KeyPressed(0x31) && (CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000)) && (player->m_bInVehicle)) {
                
                if (!pposition.x) {
                    pposition = entity->GetPosition();
                    pspeed = player->m_vecMoveSpeed;
                }

                entity->Teleport(pposition);
                player->m_vecMoveSpeed = pspeed;
                player->ApplyMoveSpeed();
                
                //Command<eScriptCommands::COMMAND_POINT_CAMERA_AT_CHAR>(player, 15, 2);
                //Command<eScriptCommands::COMMAND_SET_CHAR_HEADING>(player, 60.0f);
                //Command<eScriptCommands::COMMAND_POINT_CAMERA_AT_CHAR>(player, 1, 2);
                TheCamera.m_asCams->m_fHorizontalAngle = ch;
                TheCamera.m_asCams->m_fVerticalAngle = cv;
                
                m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;
                std::wcsncpy(weird_string, L"Loaded", 1024);
                CHud::SetHelpMessage(weird_string, true);

            } else {
                if (!vposition.x) {
                    vposition = entity->GetPosition();
                    vspeed = vehicle->m_vecMoveSpeed;
                    
                    ux = vehicle->m_matrix.up.x;
                    uy = vehicle->m_matrix.up.y;
                    uz = vehicle->m_matrix.up.z;
                    rx = vehicle->m_matrix.right.x;
                    ry = vehicle->m_matrix.right.y;
                    rz = vehicle->m_matrix.right.z;
                }

                entity->Teleport(vposition);
                vehicle->m_vecMoveSpeed = vspeed;
                
                vehicle->m_matrix.up.x = ux;
                vehicle->m_matrix.up.y = uy;
                vehicle->m_matrix.up.z = uz;
                vehicle->m_matrix.right.x = rx;
                vehicle->m_matrix.right.y = ry;
                vehicle->m_matrix.right.z = rz;

                vehicle->m_fHealth = 10000.0f;

                Command<eScriptCommands::COMMAND_RESTORE_CAMERA_JUMPCUT>();

                m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;
                std::wcsncpy(weird_string, L"Loaded", 1024);
                CHud::SetHelpMessage(weird_string, true);

            }
            
            if (KeyPressed(VK_TAB) && (CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000))) {

                if (!player->m_bInVehicle) {

                    pposition = entity->GetPosition();
                    pspeed = player->m_vecMoveSpeed;
                    ch = TheCamera.m_asCams->m_fHorizontalAngle;
                    cv = TheCamera.m_asCams->m_fVerticalAngle;
                   

                    m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;
                    std::wcsncpy(weird_string, L"Saved", 1024);
                    CHud::SetHelpMessage(weird_string, true);

                }
                else {

                    vposition = entity->GetPosition();
                    vspeed = vehicle->m_vecMoveSpeed;

                    ux = vehicle->m_matrix.up.x;
                    uy = vehicle->m_matrix.up.y;
                    uz = vehicle->m_matrix.up.z;
                    rx = vehicle->m_matrix.right.x;
                    ry = vehicle->m_matrix.right.y;
                    rz = vehicle->m_matrix.right.z;

                    m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;
                    std::wcsncpy(weird_string, L"Saved", 1024);
                    CHud::SetHelpMessage(weird_string, true);
                }
            }
            */
        };
    }
} iIILinePracticeUtil;
