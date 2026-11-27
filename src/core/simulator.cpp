#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include <iostream>
#include <string>
using namespace std;

string g_lastActionMessage="";

bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational) {
                        //making sure the target location is in the map
                        if(targetFloor<1 || targetFloor>NUM_FLOORS || targetRoom<1 || targetRoom>ROOMS_PER_FLOOR) {
                            g_lastActionMessage="[Movement Blocked] Invalid room coordinates: F" + to_string(targetFloor)
                            + "-R" + to_string(targetRoom) +".";
                            return false;
                        }
                        //making sure it is different from the current location
                        if(targetFloor==currentFloor && targetRoom==currentRoom) {
                            g_lastActionMessage = "[Movement Blocked] You are already in F"+ to_string(currentFloor)
                            + "-R" + to_string(currentRoom) + ".";
                            return false;
                        }
                        char connType= CONN_NONE;
                        //making sure the two rooms are directly connected;
                        if(!isDirectlyConnected(currentFloor,currentRoom,targetFloor,targetRoom,horizontalConn,connType)) {
                            g_lastActionMessage="[Movement Blocked] No passage connecting F" + to_string(currentFloor)
                            + "-R" + to_string(currentRoom) + " to F" + to_string(targetFloor)
                            + "-R" + to_string(targetRoom) + ".";
                            return false;
                        }
                        //making sure that traversal is allowed;
                        if(!canTraverse(targetFloor,targetRoom,connType,inventory,inventoryCount,emergencyTripsLeft,lift2Operational)) {
                            switch(connType) {
                                case CONN_SOLID_WALL: g_lastActionMessage="[Movement Blocked] A solid wall ('#') blocks your way"; break;
                                case CONN_LOCKED_DOOR: g_lastActionMessage="[Movement Blocked] The door is locked ('L'). You need the Master Key ('<')."; break;
                                case '2': g_lastActionMessage="[Movement Blocked] Lift 2 is unpowered and no emergency trips remain."; break;
                                default: g_lastActionMessage="[Movement Blocked]";
                            }
                            return false;
                        }
                        
                        string note = "";
                        if(connType=='2' && !lift2Operational && emergencyTripsLeft>0) {
                            emergencyTripsLeft--;
                            note=" [Consumed 1 emergency trip, "+ to_string(emergencyTripsLeft) + " left]";
                        }
                        //taking out energy and time for movement (will use the future function of applyresourcedelta later)
                        if(connType==CONN_DOOR || connType=='S' || connType==CONN_LOCKED_DOOR) {
                            energy -=2; time-=1;
                        }
                        else if(connType==CONN_WINDOW) {
                            energy-=3; time-=2;
                        }
                        else if(connType=='1' || connType=='2') {
                            energy-=4; time-=3;
                        }
                        //player ko location pe le ke jaa rhe hein
                        currentFloor=targetFloor;
                        currentRoom=targetRoom;
                        //in this project im going to be adding like a feedback which tell you what action you successfully recordes
                        string feedback = "[Move Success] Moved to F" + to_string(currentFloor) +
                        "-R" + to_string(currentRoom);
                        if(connType==CONN_DOOR) feedback += " through Ordinary Door ('D'). (-2 Energy, -1 Time)";
                        else if(connType==CONN_WINDOW) feedback+=" through Window ('W'). (-3 Energy, -2 Time)";
                        else if(connType==CONN_LOCKED_DOOR) feedback+=" through Unlocked Door ('L'). (-2 Energy, -1 Time)";
                        else if(connType=='S') feedback+=" via Staircase. (-2 Energy, -1 Time)";
                        else if(connType=='1') feedback+=" via Lift 1. (-4 Energy, -3 Time)";
                        else if(connType=='2') feedback+=" via Lift 2." +note+" (-4 Energy, -3 Time)";

                        g_lastActionMessage=feedback;
                        return true;


                    }
bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score) {
                            energy-=COST_INSPECT_ENERGY;
                            time-=COST_INSPECT_TIME;

                            //i created f and r index for floor n room
                            int f=currentFloor-1;
                            int r=currentRoom-1;

                            if(entityCount<=0) {
                                g_lastActionMessage = "[Inspect] F" + to_string(currentFloor) + "-R"
                                + to_string(currentRoom) + " was completely empty. (-1 Energy, -1 Time)";
                                return true;
                            }
                            string report = "[Inspect] Findings for F" + to_string(currentFloor) + "-R"
                                + to_string(currentRoom) + " (-1 Energy, -1 Time):\n";
                            for(int i=0; i < entityCount; i++) {
                                char symbol = roomEntities[f][r][i];
                                report += "  - " + string(getEntityName(symbol)) + " (')" + symbol + "')";

                                if(symbol==ENT_CLUE_4) {
                                    if(!objectives[OBJ_CLUE_4]) {
                                        objectives[OBJ_CLUE_4]=true;
                                        score+=SCORE_CLUE_FOUND;
                                        report += " -> [NEW CLUE: First digit is '4'] (+10 Score)";
                                    }
                                    else report+= " -> [Clue: First digit is '4']";
                                }
                                else if(symbol==ENT_CLUE_2) {
                                    if(!objectives[OBJ_CLUE_2]) {
                                        objectives[OBJ_CLUE_2]=true;
                                        score+=SCORE_CLUE_FOUND;
                                        report += " -> [NEW CLUE: Second digit is '2'] (+10 Score)";
                                    }
                                    else report+= " -> [Clue: Second digit is '2']";
                                }
                                else if(symbol==ENT_CLUE_7) {
                                    if(!objectives[OBJ_CLUE_7]) {
                                        objectives[OBJ_CLUE_7]=true;
                                        score+=SCORE_CLUE_FOUND;
                                        report += " -> [NEW CLUE: Third digit is '7'] (+10 Score)";
                                    }
                                    else report+= " -> [Clue: Third digit is '7']";
                                }
                                //im also going to make a report for the items that are not codes but require some item
                                else if(symbol == ENT_CONTROL_PANEL)
                                    report+= " -> [Power Terminal: Requires Fuse ('*')]";
                                else if(symbol == ENT_PASSENGER)
                                    report+=" -> [Injured Passenger : Requires Medical Kit ('%')]";
                                else if(symbol == ENT_CAT)
                                    report+= " -> [Stranded Cat: Requires Cat Carrier ('@')]";
                                else if(symbol == ENT_CABINET_TRAIN_KEY)
                                    report+= " -> [Security Cabinet: Locked (requires 3-digit code)]";
                            
                            if(i<entityCount-1) report += "\n";
                            }
                            g_lastActionMessage=report;
                            return true;
                          }
bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES]) {
                            int f = currentFloor- 1;
                            int r = currentRoom-1;

                            int itemIdx = 67;
                            for(int i=0; i<entityCount;i++) {
                                if(roomEntities[f][r][i] == itemSymbol) {
                                    itemIdx=i;
                                    break;
                                }
                            }
                            //check whether the item is in the room
                            if(itemIdx == 67) {
                                string item=""; item += itemSymbol;
                                g_lastActionMessage = "[Collect Blocked] Item '" + item + "' is not present in F"
                                + to_string(currentFloor) + "-R" + to_string(currentRoom) + ".";
                                return false;
                            }
                            //make sure the player doesnt pick up an item they cant pick up
                            if (itemSymbol == ENT_CONTROL_PANEL) {
                                g_lastActionMessage= "[Collect Blocked] Cannot pick up the Control Panel!";
                                return false;
                            }
                            if(itemSymbol==ENT_PASSENGER) {
                                g_lastActionMessage= "[Collect Blocked] The passenger is injured! Use a Medical Kit ('%').";
                                return false;
                            }
                            if(itemSymbol==ENT_CAT) {
                                g_lastActionMessage="[Collect Blocked] The cat ran away! Use the Cat Carrier ('@').";
                                return false;
                            }
                            if(itemSymbol==ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED]) {
                                g_lastActionMessage=  "[Collect Blocked] The Train Key is locked inside the security cabinet! Use UNLOCK first.";
                                return false;
                            }
                            if(!addItem(inventory, inventoryCount, itemSymbol)) {
                                g_lastActionMessage = "[Collect Blocked] Inventory is full!";
                                return false;
                            }

                            //now that all the checks are done we move onto the succesfull pick up code
                            energy-= COST_ITEM_PICKUP_ENERGY;
                            time -= COST_ITEM_PICKUP_TIME;
                          }


