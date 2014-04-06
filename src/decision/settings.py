# Maximum time a program might wait before checking for input.
EPOCH = "0.05"

settings = {    
    # THIS IS A WORK IN PROGRESS!
    "decision/master_ai": {
        "listen": [
            "sensor/vision/cam_front",
            "sensor/vision/cam_down",
            "datafeed/sanitized/accelerometer",
            "datafeed/sanitized/gyroscope",
            "datafeed/sanitized/compass",
            "datafeed/sanitized/depth",
            "movement/defuzzification"
        ],
        "desired_state": {
            "state": 0, # integer code for each state
            "task_code": "standby",
            "note": "Default Note",
            "restart": False,
            "parameter": None
        },
        "release": {
            "name": "master_ai.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "master_ai_gui.py", #"/test/task_selector.py",
            "args": ["--epoch", EPOCH]}
    },
    "decision/ai_state_machine": {
        "listen": [
            "decision/master_ai"
        ],
        "desired_state": {
            "state": 0, # integer code for each state
            "restart": False
        },
        "release": {
            "name": "ai_state_machine.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "", #"/test/task_selector.py",
            "args": ["--epoch", EPOCH]}
    },

# 'decision' left here so nothing breaks on accident. UNUSED BY AI (now)
    "decision": {
        "listen": [
          "decision/advisor",
          "sensor/vision/cam_front",
          "sensor/vision/cam_down",
          "datafeed/sanitized/accelerometer",
          "datafeed/sanitized/gyroscope",
          "datafeed/sanitized/compass",
          "datafeed/sanitized/depth",
          # XXX: Remove "datafeed/raw/depth".
          # This is for initial calibration of the depth only.
          "datafeed/raw/depth",
        ],
        "release": {
            "name": "ai_state_machine.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "ai_state_machine.py",
            "args": ["--epoch", EPOCH]},
        "depth_threshold": 550
    },
    "decision/advisor": {
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "decision/running_task": {
        "listen": [
            "datafeed/sanitized/gyroscope",
            "datafeed/sanitized/depth",
			"datafeed/sanitized/accelerometer"
            #"decision/filtering",
            #"sensor/vision/fates"
        ],
        "release": {"name": None},
        "mock": {"name": None}
    }
}

