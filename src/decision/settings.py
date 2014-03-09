# Maximum time a program might wait before checking for input.
EPOCH = "0.05"

settings = {
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
}

