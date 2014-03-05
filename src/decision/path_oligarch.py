from abc import ABCMeta, abstractmethod
import math

from base_oligarch import BaseOligarch

class PathOligarch(BaseOligarch):
    def __init__(self, communicator):
        BaseOligarch.__init__(self, communicator)

    @property
    def face_of_power(self):
        return "PathOligarch"

    def decision(self, vision_front, vision_down):
        missive = self._get_missive_template()

        if not vision_down:
            return
        theta = vision_down['theta']

        while theta < 0.0:
            theta += 2 * math.pi
        missive['desired_orientation']['yaw'] = theta
        self._publish_decision(missive)

