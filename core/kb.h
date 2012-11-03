/* Knowledge Base Class */

class KB {

    Public:
        KB::KB(); // Constructor

        // Multiple Tasks
        bool StartGateComplete;
        bool BuoyTaskComplete;
        bool ObstacleCourse1Complete;
        bool TorpedoTaskComplete;
        bool BinsTaskComplete;
        bool ObstacleCourse2Complete;

        bool TargetInRange; // Torpedos, Bins
        bool AttemptTask;

        // Start Gate  // Used for Obstacle Course Too
        bool Pillar1Seen;
        bool Pillar2Seen;
        bool Pillar1Found;
        bool Pillar2Found;

        // Paths
        bool Path1Seen;
        bool Path2Seen;
        bool Path3Seen;
        bool Path4Seen;
        bool Path5Seen;
        bool Path6Seen;
        bool Path7Seen;

        // Buoys
        bool CorrectHeading;
        bool BuoyGreenSeen;
        bool BuoyRedSeen;
        bool BuoyYellowSeen;
        bool BuoyGreenFound;
        bool BuoyRedFound;
        bool BuoyYellowFound;
        bool PrimaryBuoyComplete;
        bool SecondaryBuoyComplete;
        
        // Obstacle Course
        bool BarSeen;
        bool BarFound;

        // re use gate pillar booleans

        // Torpedoes
        bool PrimaryTorpedoTargetSeen;
        bool PrimaryTorpedoTargetComplete;
        bool SecondaryTorpedoTargetSeen;
        bool SecondaryTorpedoTargetComplete;

        // Bins
        bool BinsSeen;
        bool BinsFound;
        bool Image1Seen;
        bool Image2Seen;
        bool Image3Seen;
        bool Image4Seen;
        bool PrimaryBinTargetSeen;
        bool PrimaryBinTargetComplete;
        bool SecondaryBinTargetSeen;
        bool SecondaryBinTargetComplete;
        bool DistancesCalculated;


        // Positioning Variables
        int x;  // pixels? 
        int y;
        int z;
        int heading; // degrees

    Private:


};
