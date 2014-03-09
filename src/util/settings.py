# Maximum time a program might wait before checking for input.
EPOCH = "0.05"

settings = {
    "util/logger": {
        # The "listen" value is populated at the end of this file.
        "listen": [],
        "release": {
            "name": "logger.py",
            "args": ["--epoch", 0.005]},
        "mock": {
            "name": "logger.py",
            "args": ["--epoch", 0.005]}
    },
}

