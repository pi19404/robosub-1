from abc import ABCMeta, abstractmethod

class BaseOligarch(object):
    __metaclass__ = ABCMeta

    @abstractmethod
    def __init__(self, communicator):
        self.communicator = communicator
        # Subclasses must specify self.face_of_power

    @abstractmethod
    def decision(self, sensor, video):
        # Child classes don't need to use the same parameters.
        pass

    def _get_missive_template(self):
        return {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
                "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0},
                "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0}}

