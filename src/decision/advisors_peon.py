from base_oligarch import BaseOligarch
import math


class AdvisorsPeon(BaseOligarch):
    def __init__(self, communicator):
        BaseOligarch.__init__(self, communicator)

    @property
    def face_of_power(self):
        return "AdvisorsPeon"

    def decision(self, advice):
        missive = self._get_missive_template()
        if advice["command"] == "forward":
            missive['desired_offset']['y'] = 9001.0

        elif advice["command"] == "yaw left":
            missive['desired_orientation']['yaw'] = 3 * math.pi / 2

        elif advice["command"] == "backward":
            missive['desired_offset']['y'] = -9001

        elif advice["command"] == "yaw right":
            missive['desired_orientation']['yaw'] = math.pi / 2

        elif advice["command"] == "rise":
            missive['desired_offset']['z'] = 9001

        elif advice["command"] == "descend":
            missive['desired_offset']['z'] = -9001

        elif advice["command"] == "stop":
            pass

        elif advice["command"] == "roll left":
            missive['desired_orientation']['roll'] = 3 * math.pi / 2

        elif advice["command"] == "roll right":
            missive['desired_orientation']['roll'] = math.pi / 2

        self._publish_decision(missive)

