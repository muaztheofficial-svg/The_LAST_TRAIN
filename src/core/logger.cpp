#include "logger.h"
#include "config.h"
#include<iostream>
#include<fstring>
#include<string>
using namespace std;
bool saveGameStateToFile(const char *filename,int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,const bool
objectives[NUM_OBJECTIVES],int emergencyTripsLeft,const char roomEntitiesNUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
ofstream file(filename);
if (!file.is_open())
{
cout<<"Error: COuld not save game to "<<filename<<endl;
return false;}
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
for (int e=0;e<entityCOunts[f][r];e++)
{
file<<" "<<roomEntities[f][r][e];}
file<<endl;}}
file.close();
return true;}
bool loadGameStateFromFile(const char *filename,int &playerFloor,int &playerRoom,int &energy,int &time,int &score,char inventory[MAX_INVENTORY_CAPACITY],int &inventoryCount,bool objecives[NUM_OBJECTIVES],int &emergencyTripsLeft,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
ifstream file(filename);
if (!file.is_open())
{
cout<<"Error: COuld not load game from "<<filename<endl;
return false;}
if (!file>>playerFloor>>playerRoom)
{
file.close();
return false;}
if (!file>>energy>>time>>score>>emergencyTripsLeft)
{
file.close();
return false;}
if (!(file>>inventoryCount))
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
file>>entityCount[f][r];
for (int e=0;e<entityCounts[f][r];e++)
{
file>>roomEntities[f][r][e];}
for (int e=entityCounts[f][r];e<MAX_ENTITIES_PER_ROOM;e++)
{
roomEntities[f][r][e]=' ';}}}
file.close();
return true;


// File persistence for savegame.txt and game_summary.txt using primitive variables
