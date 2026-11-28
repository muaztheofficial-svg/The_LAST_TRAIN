#include "logger.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

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
                                for(int i=0; i<NUM_OBJECTIVES;i++) {
                                    //for the objective of mission failed dont print that line if the mission has not failed
                                    if(i==OBJ_MISSION_FAILED && !objectives[OBJ_MISSION_FAILED]) continue;
                                    if(objectives[i]) 
                                        report << " [X] "<< description[i] << endl;
                                    else report << " [ ] "<< description[i] << endl;

                                }
                                report << "============================================================" << endl;
                                report.close();
                            }

