/* Knowledge Base Class */

class KB {

    public:
        KB(); // Constructor
        int updateKB(); 


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
        bool Pillar1Found;
        bool Pillar2Found;

        // Paths


        // Buoys
        bool CorrectHeading;
        bool buoyPrimaryFound;
        bool buoySecondaryFound;
        bool PrimaryBuoyComplete;
        bool SecondaryBuoyComplete;
        
        // Obstacle Course
        bool BarFound;

        // re use gate pillar booleans

        // Torpedoes
        bool PrimaryTorpedoTargetComplete;
        bool SecondaryTorpedoTargetComplete;

        // Bins
        bool BinsFound;
        bool BinsPrimaryFound;
        bool BinsSeondaryFound;
        bool PrimaryBinTargetComplete;
        bool SecondaryBinTargetComplete;
        bool DistancesCalculated;

        // Positioning Variables
        // Center point for primary target

        int x1;  // pixels? 
        int y1;
        int z1;
        int heading1; // degrees

        // Center point for secondary target 
        int x2;
        int y2;
        int z2;
        int heading2;
        

        // Predefined at competition time
        // Buoy Targets
        colors buoyPrimary;
        colors buoySecondary;
        
        // Torpedo Targets
        colors torpedoPrimary;
        colors torpedoSecondary;

        // Bin Targets
        images binPrimary;
        images binSecondary;


    private:


};
