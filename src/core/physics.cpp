#include "physics.h"
#include "util.h"
#include "state.h"
#include "switches.h"

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol) {
    for(int i=0;i<InventoryCount;i++)
        if(inventory[i]=itemSymbol)
            return true;
    return false;
}

bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType) {
                            int floorIdx;
                            outConnType= CONN_NONE;
    
                            if(fromF==toF) { //horizontal movement ;)
                                floorIdx= fromF-1;
                                if((fromF==1 || fromF==2)&&((fromR==2 && toR==3)||(fromR==3 && toR==2))) {
                                    outConnType='1'; //i used 1 for this horizontal movement through lift 1 as there is no conn type for lift 1 in state.h
                                    return true;
                                }
                                if(toR==fromR+1) {//moving right
                                    char c = horizontalConn[floorIdx][fromR-1];
                                    if( c != CONN_NONE) {
                                        outConnType=c; return true;
                                    }
                                    else return false;
                                }
                                if(toR==fromR-1) { //moving left
                                    char c=horizontalConn[floorIdx][toR-1];
                                    if(c!=CONN_NONE) {
                                        outConnType=c;
                                        return true;
                                    }
                                    else return false;
                                }
                                return false;
                        }
                        if(fromR==1 && toR==1) { //staircases
                            if((fromF==1 && toF==2)||(fromF==2 && toF==1)||(fromF==2 && toF==3)||(fromF==3 && toF==2)||(fromF==4 && toF==5)||(fromF==5 && toF==4)) {
                                outConnType='S'; //i used 'S' as a conn becuz there was no conn for stair case in state.h
                                return true;
                        }
                    }
                    //lift 1
                    if((fromR==2 || fromR==3) && (toR==2 || toR==3)) {
                        if(canUseLift1(fromF,toF)) {
                            outConnType='1';
                            return true;
                        }
                    }
                    //lift 2
                    if(fromR==4 && toR==4) {
                        if((fromF==2 || fromF==4 || fromF==5) &&
                        (toF==2 || toF==4 || toF==5) &&
                        fromF!=toF) {
                            outConnType='2';
                            return true;
                        }
                    }
                    return false;
                }
bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational) {
                    if(targetRoom<1 || targetRoom>ROOMS_PER_FLOOR) return false;
                    if(connType==CONN_SOLID_WALL) return false;
                    if(connType==CONN_LOCKED_DOOR) return unlockFloor3Door(inventory,inventoryCount);
                    if(connType=='1') return true;
                    if(connType=='2') {
                        int fromFloor;
                        if(targetFloor==2) fromFloor=5;
                        else fromFloor=2; 
                        return canUseLift2(fromFloor,targetFloor,lift2Operational,emergencyTripsLeft);
                    }
                    if(connType==CONN_DOOR || connType==CONN_WINDOW || connType=='S') return true;
                    return false;
                }

