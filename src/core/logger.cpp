#include "logger.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <string>
#include "config.h"
using namespace std;
bool saveGameStateToFile(const char *filename,int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,const bool
objectives[NUM_OBJECTIVES],int emergencyTripsLeft,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
ofstream file(filename);
if (!file.is_open())
{
cout<<"Error: COuld not save game to "<<filename<<endl;
return false;}
file<<playerFloor<<" "<<playerRoom<<endl; //why does abdullah leave so many bugs :sigh:
file<<energy<<" "<<time<<" "<<score<<" "<<emergencyTripsLeft<<endl;
file<<inventoryCount;
for (int i=0;i<inventoryCount;i++)
{
file<<" "<<inventory[i];}
file<<endl;
for (int i=0;i<NUM_OBJECTIVES;i++)
{
file<<(objectives[i]?1:0);
if (i<NUM_OBJECTIVES-1)file<<" ";}
file<<endl;
for (int f=0;f<NUM_FLOORS;f++)
{
for (int r=0;r<ROOMS_PER_FLOOR;r++)
{
file<<entityCounts[f][r];
for (int e=0;e<entityCounts[f][r];e++)
{
file<<" "<<roomEntities[f][r][e];}
file<<endl;}}
file.close();
return true;}
bool loadGameStateFromFile(const char *filename,int &playerFloor,int &playerRoom,int &energy,int &time,int &score,char inventory[MAX_INVENTORY_CAPACITY],int &inventoryCount,bool objectives[NUM_OBJECTIVES],int &emergencyTripsLeft,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) 
{
ifstream file(filename);
if (!file.is_open())
{
cout<<"Error: COuld not load game from "<<filename<<endl;
return false;}
if (!file>>playerFloor>>playerRoom)
{
file.close();
return false;}
if (!file>>energy>>time>>score>>emergencyTripsLeft)
{
file.close();
return false;}
if (!(file>>inventoryCount)){
file.close();
return false;}
for (int i=0;i<inventoryCount;i++)
{
file>>inventory[i];}
for (int i=inventoryCount;i<MAX_INVENTORY_CAPACITY;i++)
{
inventory[i]=' ';}
for (int i=0;i<NUM_OBJECTIVES;i++)
{
int flag=0;
file>>flag;
objectives[i]=(flag==1);}
for (int f=0;f<NUM_FLOORS;f++)
{
for (int r=0;r<ROOMS_PER_FLOOR;r++)
{
file>>entityCounts[f][r];
for (int e=0;e<entityCounts[f][r];e++)
{
file>>roomEntities[f][r][e];}
for (int e=entityCounts[f][r];e<MAX_ENTITIES_PER_ROOM;e++)
{
roomEntities[f][r][e]=' ';}}}
file.close();
return true;}

// File persistence for savegame.txt and game_summary.txt using primitive variables
void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory) {

                                ofstream report(filename);
                                
                                report<< "============================================================\n";
                                report<< "              THE LAST TRAIN - GAME SUMMARY REPORT          \n";
                                report<< "============================================================\n";
                                if(victory) report << "Outcome         : VICTORY\n";
                                else report << "Outcome         : DEFEAT\n";
                                report<< "Final Score     : " << score << " points\n";
                                report<< "Remaining Energy: " << remainingEnergy << " / " << MAX_ENERGY << "\n";
                                report<< "Remaining Time  : " << remainingTime << " / " << MAX_TIME << "\n";
                                report<< "============================================================\n";
                                report<< "                     MISSION CHECKLIST                      \n";
                                report<< "============================================================\n";
                                
                                string description[NUM_OBJECTIVES] = {
                                    "Collect Master Key","Find Clue Digit 4","Find Clue Digit 2",
                                    "Find Clue Digit 7","Unlock Cabinet in F5-R1","Collect Train Key",
                                    "Collect Fuse","Install Fuse in Control Panel (F3-R4)","Collect Lift Pass",
                                    "Activate Lift 2","Rescue Stranded Passenger","Collect Cat Carrier",
                                    "Secure Cat in Carrier","Prepare Train for Departure","Mission Victory","Mission Failed"
                                };
                                for (int i=0; i<NUM_OBJECTIVES;i++) {
                                    //for the objective of mission failed dont print that line if the mission has not failed
                                    if(i==OBJ_MISSION_FAILED && !objectives[OBJ_MISSION_FAILED]) continue;
                                    if(objectives[i]) 
                                        report << " [X] "<< description[i] << endl;
                                    else report << " [ ] "<< description[i] << endl;

                                }
                                report << "============================================================" << endl;
                                report.close();
                            }
                        

