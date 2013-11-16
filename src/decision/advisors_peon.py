from base_oligarch import BaseOligarch
import math


class AdvisorsPeon(BaseOligarch):
    def __init__(self, communicator):
        BaseOligarch.__init__(self, communicator)
        self.face_of_power = "AdvisorsPeon"

    def decision(self, sensor, video, advice):
        missive = self.get_missive_template()
        if advice["command"] == "forward":
            missive['desired_offset']['y'] = 9001.0
        elif advice["command"] == "yaw left":
            missive['desired_orientation']['z'] = 3 * math.pi / 2
        elif advice["command"] == "backward":
            missive['desired_offset']['y'] = -9001
        elif advice["command"] == "yaw right":
            missive['desired_orientation']['z'] = math.pi / 2
        elif advice["command"] == "rise":
            missive['desired_offset']['z'] = 9001
        elif advice["command"] == "descend":
            missive['desired_offset']['z'] = -9001
        elif advice["command"] == "stop":
            pass
        elif advice["command"] == "roll left"
            missive['desired_orientation']['roll'] = 3 * math.pi / 2
        elif advice["command"] == "roll right"
            missive['desired_orientation']['roll'] = math.pi / 2

        return missive

