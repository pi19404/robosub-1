from abc import ABCMeta, abstractmethod

from base_oligarch import BaseOligarch

class PathOligarch(BaseOligarch):
    __metaclass__ = ABCMeta

    def __init__(self, communicator):
        BaseOligarch.__init__(self, communicator)
        self.face_of_power = "PathOligarch"

    def decision(self, sensor, video):
        self._publish_decision(self._get_missive_template())

    def _get_missive_template(self):
        return {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
                "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0},
                "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0}}

