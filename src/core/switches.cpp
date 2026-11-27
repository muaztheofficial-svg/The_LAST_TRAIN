#include "switches.h"
#include "util.h"
#include "physics.h"
#include<iostream>
using namespace std;
bool canUseLift1(int fromFloor, int toFloor)
{
if ((fromFloor==1 && toFloor==2) || (fromFloor==2 && toFloor==1))
{
return true;}
return false;}
bool canUseLift2(int fromFloor,int toFloor,bool lift2Operational,int emergencyTripsLeft)
{
bool validFrom=(fromFloor==2 || fromFloor==4 || fromFloor==5);
bool validTo=(toFloor==2 || toFloor==4 || toFloor==5);
if (!validFrom || !validTo || fromFloor==toFloor)
{
return false;}
return (lift2Operational || emergencyTripsLeft>0);}
bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount)
{
return hasItem(inventory,inventoryCount,ENT_MASTER_KEY);}

//Muaz's code as you can tell by the sane formatting
bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES]) {
    //first we check agr pehle se cabinet code ka obj pura hogya hai
    if(objectives[OBJ_CABINET_UNLOCKED]) return true;
    if(stringEquals(enteredCode,"427"))
        return true;
    return false;
}
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score) {
                                if(!hasItem(inventory,inventoryCount,ENT_FUSE)) return false;
                                if(removeItem(inventory,inventoryCount,ENT_FUSE)) {
                                    if(!objectives[OBJ_FUSE_INSTALLED]) {
                                        objectives[OBJ_FUSE_INSTALLED]= true;
                                        objectives[OBJ_LIFT2_ACTIVE]= true;
                                        score += 20;
                                    }
                                    return true;
                                }
                                return false;
                                    
                             }



// Door lock mechanics, lifts, and cabinet verification without structures
