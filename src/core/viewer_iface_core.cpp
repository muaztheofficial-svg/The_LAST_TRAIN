#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
void renderDashboard(int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,int emergencyTripsLeft)
{
cout<<"============================================================"<<endl;
cout<<"                      PLAYER DASHBOARD                      "<<endl;
cout<<"============================================================"<<endl;
cout<<"Location: Floor "<<playerFloor<<", Room "<<playerRoom<<" (\033[1;32m[P]\033[0m)"<<endl;
cout<<"Energy : "<<energy<<" / "<<MAX_ENERGY<<" | TIme : "<<time<<" / "<<MAX_TIME<<" | Score : "<<score<<endl;
cout<<" Emergency Trips Left on Lift 2: "<<emergencyTripsLeft<<endl;
cout<<" Inventory ("<<inventoryCount<<"/"<<MAX_INVENTORY_CAPACITY<< "): ";
if (inventoryCount==0)
{
cout<<"[Empty]"<<endl;}
else
{
cout<<"[ ";
for (int i=0;i<inventoryCount;i++)
{
cout<<inventory[i]<<" ( "<<getEntityName(inventory[i])<<")";
if (i<inventoryCount-1)
cout<<", ";}
cout<<" ]"<<endl;}
cout<<"============================================================"<<endl;}
void renderRoomDetails(int floor,int room,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCount)
{
s_roomEntities=roomEntities;
cout<<"--- Current ROom: F"<<floor<<"-R"<<room<<" ---"<<endl;
if (entityCount<=0)
{
cout<<" Items Present: None"<<endl;}
else
{
cout<<" Items Present ("<<entityCount<<"):"<<endl;
int f=floor-1;
int r=room-1;
for (int i=0;i<entityCount;i++)
{
char symbol=roomEntities[f][r][i];
cout<<"   - ["<<symbol<<"] "<<getEntityName(symbol)<<endl;}}}
// Terminal UI and dashboard rendering using primitive parameters
