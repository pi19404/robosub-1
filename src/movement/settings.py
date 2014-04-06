EPOCH = '0.05'

settings = {
    "movement/orientation": { 
        "constants": {"accel_constant": .95, "gyro_constant": .05},
        "listen": [
            "datafeed/sanitized/accelerometer",
            "datafeed/sanitized/gyroscope",
            "datafeed/sanitized/compass"
        ],
        "release": {
            "name": "stabilization/complementary_filter.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/stabilization": { 
        "stabilize": True,
        "listen": [
            "decision",
            "datafeed/sanitized/depth",
            "movement/orientation"
        ],
        # kP, kI, and kD are tuning variables. They determine the magnitude of the effect the P, I, and D terms have on the output.
        # Max and Min Integrator governs the total effect the Integrator term can have on the output.
        # The names of these must coorespond to the name of the direction or orientation in
        # the incoming ai packet
        "PID_Settings": {"roll": {"kP": 0.1, "kI": 0.0, "kD": 0.0, "Min_I": -20.0, "Max_I": 20.0},
                         "up/down": {"kP": 0.1, "kI": 0.0, "kD": 0.0, "Min_I": -20.0, "Max_I": 20.0}},
        "release": {
            "name": "stabilization/stabilizer.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/fuzzification": {
        #"ip": "10.99.66.129", # IP address of computer with ps3 controller
        "listen": [
            "movement/stabilization"
        ],
        # The N values in each fuzzy set will define N + 1 regions.
        # The first number represents an x value whlie the second number
        # represents a y value. All points left of the lowest x value will have
        # the same y value. The regions between specified points will have
        # a value determined by connecting a straight line between those points
        # All points to the right of the highest value have the same y value.
        # For example, the values
        # [[-3.0, 1.0], [-1.0, 0.5], [0.0, 0.0], [2.0, 1.0], [3.0, 0.0]] will
        # look (something) like:
        #-----------\                     1.0           /                      
        #            ---\                 |            / \                     
        #                ---\             |          /    \                    
        #                    ---\         |         /      \                   
        #                        ---\     0.5     /         \                  
        #                            \    |      /           \                 
        #                             \   |    /              \                
        #                              \  |   /                \               
        #                               \ | /                   \              
        #        ------------------------\-/---------------------\-------------
        #-4.0    -3.0    -2.0    -1.0     0.0    1.0    2.0    3.0
        "fuzzy_sets": {
            "is_right": [[0.0, 0.0], [2.0, 1.0]],
            "is_left": [[-2.0, 1.0], [0.0, 0.0]],
            "is_forward": [[0.0, 0.0], [2.0, 1.0]],
            "is_back": [[-2.0, 1.0], [0.0, 0.0]]
        },
        "release": {
            "name": "fuzzy_logic_fuzzifier.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "/test/xbox_controller.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/defuzzification": {
        "listen": [
            "movement/fuzzification",
            "datafeed/raw/accelerometer"
        ],
        "release": {
            "name": "defuzzifier.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "slider1.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/translation": {
        #"ip": "192.168.1.7", # IP address of person with slider control
        "listen": [
            "movement/defuzzification"
        ],
        "thresholds": {
            "front_left": {
                # TODO we may need to specify different numbers for forward vs
                # backward.
                "positive": [1, 127],
                "negative": [1, 127],
                # Multiplier may flip the motor direction for motors that are
                # wired backwards. Note that multiplier is applied before the
                # positive and negative thresholds are applied, so positive and
                # negative are the same direction for each thruster regardless
                # of wiring.
                "multiplier": -1
            },
            "front_right": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "middle_left": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "middle_right": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "back_left": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "back_right": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": -1
            }
        },
        "release": {
            "name": "translater.py",
            "args": ["--epoch", EPOCH]
        },
        "mock": {
            "name": "slider2.py",
            "args": ["--epoch", EPOCH]
        },
    },
}

