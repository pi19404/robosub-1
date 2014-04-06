# task codes!

class TaskCode():
    INVALID = -1
    
    STANDBY = 0
    STANDBY_AT_DEPTH = 1
    HOLD_DEPTH = 2

    DIVE = 3
    FOO_SQUARE = 4
    START_GATE = 5
    LINE_FOLLOW = 6
    
    
    @staticmethod
    def get_task_name(task_code):
        if task_code == TaskCode.STANDBY:
            return "standby"
        elif task_code == TaskCode.FOO_SQUARE:
            return "foo square"
        elif task_code == TaskCode.START_GATE:
            return "start gate"
        elif task_code == TaskCode.LINE_FOLLOW:
            return "line follow"
        elif task_code == TaskCode.STANDBY_AT_DEPTH:
            return "hold depth (standby)"
        elif task_code == TaskCode.DIVE:
            return "dive"
        else:
            return "unknown"
        
