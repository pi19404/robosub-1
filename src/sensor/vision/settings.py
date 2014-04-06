stream_port_start = 50110
# Be careful with port span.
# Number of ports to use in a video stream. 1 port for control, 2**x for
# sending images.
stream_port_span = (2**5) + 1

def gen_stream_port(start, span):
    port = start
    while True:
        yield port
        port += span

stream_port_gen = gen_stream_port(stream_port_start, stream_port_span)

settings = {
    'sensor/vision/plugin/Streamer': {
        'command_map': {
            '1': {
                'func': lambda fp : fp.im,
                'hint': 'Raw image'
            },
            '2': {
                'func': lambda fp : fp.hsv,
                'hint': 'HSV'
            },
            'q': {
                'func': lambda fp : fp.im_blue,
                'hint': 'Blue channel'
            },
            'w': {
                'func': lambda fp : fp.im_green,
                'hint': 'Green channel'
            },
            'e': {
                'func': lambda fp : fp.im_red,
                'hint': 'Red channel'
            },
            'r': {
                'func': lambda fp : fp.im_hue,
                'hint': 'Hue channel'
            },
            't': {
                'func': lambda fp : fp.im_saturation,
                'hint': 'Saturation channel'
            },
            'y': {
                'func': lambda fp : fp.im_value,
                'hint': 'Value channel'
            },
            'a': {
                'func': lambda fp : fp.filtered_red,
                'hint': 'InRange red'
            },
            's': {
                'func': lambda fp : fp.filtered_orange,
                'hint': 'InRange orange'
            },
            'd': {
                'func': lambda fp : fp.filtered_yellow,
                'hint': 'InRange yellow'
            },
            'f': {
                'func': lambda fp : fp.filtered_green,
                'hint': 'InRange green'
            },
            'g': {
                'func': lambda fp : fp.filtered_blue,
                'hint': 'InRange blue'
            },
            'z': {
                'func': lambda fp : fp.flooded_red,
                'hint': 'InRange red'
            },
            'x': {
                'func': lambda fp : fp.flooded_orange,
                'hint': 'InRange orange'
            },
            'c': {
                'func': lambda fp : fp.flooded_yellow,
                'hint': 'InRange yellow'
            },
            'v': {
                'func': lambda fp : fp.flooded_green,
                'hint': 'InRange green'
            },
            'b': {
                'func': lambda fp : fp.flooded_blue,
                'hint': 'InRange blue'
            },
            'i': {
                'func': lambda fp : fp.hud,
                'hint': 'Hud'
            },
        }
    },
    'sensor/vision/control': {
        #'port': 20054,
        'listen': [],
        'release': {'path': None},
        'mock': {'path': None},
        'maintenance_interval': 5,
        'max_failed_frames': 150,
        'vision_processors': [
            'sensor/vision/cam_down',
            #'sensor/vision/cam_front'
        ]
    },
    'sensor/vision/cam_fake': {
        'name': 'cam_fake',
        'recorded_video': '/home/cevans/Videos/RoboSub/Obstacles/pathOutOfStartGate640x480.mp4',
        'recorded_video_': '/home/cevans/Videos/RoboSub/Obstacles/tollBooth640x480.mp4',
        'stream_port': stream_port_gen.next(),
        'port_span': stream_port_span,
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
        'plugins': ['Path'] # Tester module can be added for debug
    },
    'sensor/vision/cam_front': {
        'name': 'cam_front',
        'stream_port': stream_port_gen.next(),
        'port_span': stream_port_span,
        'stream_type': 'device',
        'symlink': '/dev/cam_front',
        'codec': 'MJPG',
        'enable': True,
        'log': [
            'raw',
            'processed'
        ],
        'fps': 10,
        'width': 640,
        'height': 480,
        'release': {'path': None},
        'mock': {'path': None},
        'plugins': []
    },
    'sensor/vision/cam_down': {
        'name': 'cam_down',
        'stream_port': stream_port_gen.next(),
        'port_span': stream_port_span,
        'stream_type': 'device',
        'symlink': '/dev/cam_down',
        'codec' : 'MJPG',
        'enable': False,
        'log': [
            'raw',
            'processed'
        ],
        'fps': 10,
        'width': 640,
        'height': 480,
        'release': {'path': None},
        'mock': {'path': None},
        'plugins': ['Streamer']
    },
    'sensor/vision/cam_left': {
        'symlink': '/dev/cam_left',
        'stream_port': stream_port_gen.next(),
        'port_span': stream_port_span,
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
        'stream_port': stream_port_gen.next(),
        'port_span': stream_port_span,
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
