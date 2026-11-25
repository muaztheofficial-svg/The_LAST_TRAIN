#include "level.h"
#include "util.h"
#include "config.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// Modular implementation using primitive arrays for map parsing and verification
const char* getEntityName(char entitySymbol) {
    switch(entitySymbol) {
        case ENT_MASTER_KEY: return "Master Key";
        case ENT_ENERGY_PACK: return "Energy Pack";
        case ENT_EMERGENCY_TOOLKIT: return "Emergency Toolkit";
        case ENT_CLUE_4: return "Clue Digit 4";
        case ENT_CLUE_2: return "Clue Digit 2";
        case ENT_CLUE_7: return "Clue Digit 7";
        case ENT_FUSE: return "Fuse";
        case ENT_CAT: return "Cat";
        case ENT_CONTROL_PANEL: return "Control Panel Terminal";
        case ENT_LIFT_PASS: return "Lift Pass";
        case ENT_PASSENGER: return "Stranded Passenger";
        case ENT_MEDICAL_KIT: return "Medical Kit";
        case ENT_EMERGENCY_CARD: return "Emergency Access Card";
        case ENT_CABINET_TRAIN_KEY: return "Cabinet / Train Key";
        case ENT_CAT_CARRIER: return "Cat Carrier";
        case ENT_OBSERVATION_TOKEN: return "Observation Token";
        default: return "cringe";
    }

}

bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) {
                        
                        ifstream file(filename);
                        if (!file.is_open()) {
                            cout << "Error opening file " << filename << endl;
                            return false;
                        }

                        for(int f=0;f<NUM_FLOORS;f++) {
                            for(int r=0 ; r<ROOMS_PER_FLOOR -1;r++) {
                                horizontalConn[f][r]= CONN_NONE ;
                            }
                            for(int r=0 ; r<ROOMS_PER_FLOOR; r++) {
                                entityCounts[f][r]=0;
                                for(int e=0; e < MAX_ENTITIES_PER_ROOM; e++) {
                                    roomEntities[f][r][e] = ENT_NONE;
                                }
                            }
                        }
                    string line[17];
                    for(int i=0;i<17;i++) {
                        if(!getline(file,line[i])) {
                            file.close();
                            return false;
                        }
                    }
                    for(int f=0;f<NUM_FLOORS;f++) {
                        const string &labelLine = line[14 - 3*f]; //becuz the connection label is on 14-3*f for every floor f
                        const string &itemLine = line[15 - 3*f]; //room items are on the line below label
                        
                        horizontalConn[f][0] = labelLine[20];

                        char c = labelLine[32]; //room 1
                        //room 2
                        if(c==CONN_DOOR || c==CONN_WINDOW || c==CONN_LOCKED_DOOR || c==CONN_SOLID_WALL )
                            horizontalConn[f][1]=c;
                        else
                            horizontalConn[f][1]=CONN_NONE;
                        horizontalConn[f][2]=labelLine[44]; //room 3
                        
                        int colStart[4] = {9,21,33,45};
                        int colEnd[4] = {19,31,43,55};

                        for(int r=0; r<ROOMS_PER_FLOOR; r++) {
                            for(int col=colStart[r]; col<=colEnd[r];col++) {
                                char a=itemLine[col];
                                if(a!=ENT_NONE) {
                                    roomEntities[f][r][entityCounts[f][r]]=a;
                                    entityCounts[f][r]++;
                                }
                            }
                        }
                    }
                    return true;
}

