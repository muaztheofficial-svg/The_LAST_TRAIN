#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
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

