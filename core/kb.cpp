/* Knowledge Base cpp */

#include "kb.h"

KB::KB()
{
    
        // Multiple Tasks
        bool StartGateComplete = false;
        bool BuoyTaskComplete = false;
        bool ObstacleCourse1Complete = false;
        bool TorpedoTaskComplete = false;
        bool BinsTaskComplete = false;
        bool ObstacleCourse2Complete = false;

        bool TargetInRange = false; // Torpedos, Bins
        bool AttemptTask = false;

        // Start Gate  // Used for Obstacle Course Too
        bool Pillar1Seen = false;
        bool Pillar2Seen = false;
        bool Pillar1Found = false;
        bool Pillar2Found = false;

        // Paths
        bool Path1Seen = false;
        bool Path2Seen = false;
        bool Path3Seen = false;
        bool Path4Seen = false;
        bool Path5Seen = false;
        bool Path6Seen = false;
        bool Path7Seen = false;

        // Buoys
        bool CorrectHeading = false;
        bool BuoyGreenSeen = false;
        bool BuoyRedSeen = false;
        bool BuoyYellowSeen = false;
        bool BuoyGreenFound = false;
        bool BuoyRedFound = false;
        bool BuoyYellowFound = false;
        bool PrimaryBuoyComplete = false;
        bool SecondaryBuoyComplete = false;
        
        // Obstacle Course
        bool BarSeen = false;
        bool BarFound = false;

        // re use gate pillar booleans

        // Torpedoes
        bool PrimaryTorpedoTargetSeen = false;
        bool PrimaryTorpedoTargetComplete = false;
        bool SecondaryTorpedoTargetSeen = false;
        bool SecondaryTorpedoTargetComplete = false;

        // Bins
        bool BinsSeen = false;
        bool BinsFound = false;
        bool Image1Seen = false;
        bool Image2Seen = false;
        bool Image3Seen = false;
        bool Image4Seen = false;
        bool PrimaryBinTargetSeen = false;
        bool PrimaryBinTargetComplete = false;
        bool SecondaryBinTargetSeen = false;
        bool SecondaryBinTargetComplete = false;
        bool DistancesCalculated = false;

        // Positioning Variables
        int x = 0;  // pixels? 
        int y = 0;
        int z = 0;
        int heading = 0; // degrees
}
