port_start = 50000
port_span = 15 # Number of ports to use in a video stream.

def gen_stream_port(start, span):
    port = start
    while True:
        yield port
        port += span

port_gen = gen_stream_port(port_start, port_span)

settings = {
    'sensor/vision/control': {
        #'port': 20054,
        'listen': [],
        'release': {'path': None},
        'mock': {'path': None},
        'maintenance_interval': 5,
        'max_failed_frames': 150,
        'vision_processors': [
            'sensor/vision/cam_down'
        ]
    },
    'sensor/vision/cam_fake': {
        'name': 'cam_fake',
        'recorded_video': '/home/cevans/Videos/RoboSub/Obstacles/pathOutOfStartGate640x480.mp4',
        'recorded_video_': '/home/cevans/Videos/RoboSub/Obstacles/tollBooth640x480.mp4',
        'stream_port': port_gen.next(),
        'port_span': port_span,
        'stream_type': 'recorded_video',
        'codec' : 'MJPG',
        'enable': True,
        'log': [
            'raw',
            'processed'
        ],
        'fps': 8,
        'width': 640,
        'height': 480,
        'release': {'path': None},
        'mock': {'path': None},
        'plugins': ['VideoLogger', 'Path']
    },
    'sensor/vision/cam_front': {
        'name': 'cam_front',
        'stream_port': port_gen.next(),
        'port_span': port_span,
        'stream_type': 'device',
        'symlink': '/dev/cam_front',
        'codec': 'MJPG',
        'enable': True,
        'log': [
            'raw',
            'processed'
        ],
        'fps': 5,
        'width': 640,
        'height': 480,
        'release': {'path': None},
        'mock': {'path': None},
        'plugins': []
    },
    'sensor/vision/cam_down': {
        'name': 'cam_down',
        'stream_port': port_gen.next(),
        'port_span': port_span,
        'stream_type': 'device',
        'symlink': '/dev/cam_down',
        'codec' : 'MJPG',
        'enable': False,
        'log': [
            'raw',
            'processed'
        ],
        'fps': 5,
        'width': 640,
        'height': 480,
        'release': {'path': None},
        'mock': {'path': None},
        'plugins': ['Streamer', 'Tester']
    },
    'sensor/vision/cam_left': {
        'symlink': '/dev/cam_left',
        'stream_port': port_gen.next(),
        'port_span': port_span,
        'codec' : 'MJPG',
        'enable': False,
        'log': [
            'raw',
            'processed'
        ],
        'fps': 5,
        'width': 640,
        'height': 480,
        'release': {'path': None},
        'mock': {'path': None},
        'plugins': []
    },
    'sensor/vision/cam_right': {
        'symlink': '/dev/cam_right',
        'stream_port': port_gen.next(),
        'port_span': port_span,
        'codec' : 'MJPG',
        'enable': False,
        'log': [
            'raw',
            'processed'
        ],
        'fps': 5,
        'width': 640,
        'height': 480,
        'release': {'path': None},
        'mock': {'path': None},
        'plugins': []
    }
}
