#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//so basically to remove items from building.txt which have been picked up we will introduce a pointer to room entities array to check which items were collected;
static const char (*s_roomEntities)[ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM]= nullptr;

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
s_roomEntities = roomEntities;
cout<<"--- Current Room: F"<<floor<<"-R"<<room<<" ---"<<endl;
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
//Muaz's two functions below:

//i used an ANSI escape sequence to turn the player green for better visibility
void renderAsciiBuilding(int playerFloor, int playerRoom) {
    cout<< "\n--- Building Layout (\033[1;33m[P]\033[0m = Your Current Location) ---\n";
    ifstream file("data/building.txt");
    if(!file.is_open()) {
        cout << "Error opening file for Rendering the map\n";
        return;
    }

    string line[17];
    for(int i=0;i<17;i++) {
        getline(file, line[i]);
    }
    file.close();

    //making a string variable to look for the players location
    string targetLabel= "F" + to_string(playerFloor) + "-R" + to_string(playerRoom);
    string playerMarker = "\033[1;33m[P]  \033[0m";
    int lineIndex=14-3*(playerFloor-1); //this formula gives the line where floors and rooms are written for index(0-4)

    int position = line[lineIndex].find(targetLabel);
    if(position != -1) line[lineIndex].replace(position,5,playerMarker);

    //now im going to add the logic for removing picked up items from the building
    if(s_roomEntities!=nullptr) {
        int colStarts[4]={9,21,33,45};
        int colEnds[4]={19,31,43,55};
        for(int f=0;f<NUM_FLOORS;f++) {
            int itemLine = 15-3*f;
            for(int r=0;r<ROOMS_PER_FLOOR; r++) {
                char roomItems[MAX_ENTITIES_PER_ROOM];
                int count=0;
                for(int k=0;k<MAX_ENTITIES_PER_ROOM;k++) {
                    char item=s_roomEntities[f][r][k];
                    if(item != ENT_NONE && item!='\0')
                        roomItems[count++] = item;
                }
                //checking the entities in this room . meaning column range
                for(int col=colStarts[r]; col<=colEnds[r];col++) {
                    char ch =line[itemLine][col];
                    if(ch != ENT_NONE) {
                        bool isitemstillthere = false;
                        for(int k=0;k<count;k++) {
                            if(roomItems[k] == ch) {
                                isitemstillthere = true;
                                roomItems[k] = ENT_NONE; // matched one copy
                                break;
                            }
                        }
                        //for items no longer there we will erase it with one space;
                        if(!isitemstillthere) line[itemLine][col] = ENT_NONE;
                    }
                }
            }
        }
    }
    
    for(int i=0;i<17;i++) {
        cout<<line[i]<<endl;
    }
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]) {
    cout <<endl<<"============================================================"<<endl;
    cout << "                   OBJECTIVES CHECKLIST                     "<<endl;
    cout << "============================================================"<<endl;

    string descriptions[NUM_OBJECTIVES] = {
    "Collect Master Key","Find Clue Digit 4",
        "Find Clue Digit 2","Find Clue Digit 7","Unlock Cabinet in F5-R1",
        "Collect Train Key","Collect Fuse","Install Fuse in Control Panel (F3-R4)",
        "Collect Lift Pass","Activate Lift 2","Rescue Stranded Passenger","Collect Cat Carrier",
        "Secure Cat in Carrier","Prepare Train for Departure",
        "Mission Victory","Mission Failed"
    };
    for(int i=0;i<NUM_OBJECTIVES;i++) {
        if(i==OBJ_MISSION_FAILED && objectives[i]==0)
            continue; //this will ignore failure unless the mission has actually failed
        if(objectives[i]==1) cout<<"[X]";
        else cout<<"[ ]";
        cout<<descriptions[i]<<endl;
    }
    cout << "============================================================"<<endl;
}
