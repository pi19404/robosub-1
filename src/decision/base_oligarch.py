from abc import ABCMeta, abstractmethod

class BaseOligarch(object):
    __metaclass__ = ABCMeta

    @abstractmethod
    def __init__(self, communicator):
        self.communicator = communicator
        # Subclasses must specify self.face_of_power

    @abstractmethod
    def decision(self, *args):
        # Child classes don't need to use the same parameters.
        # Must call self._publish_decision().
        # Also, the parameters to this function will be the most recently
        # cached values seen by the state machine. They might be sent
        # to this method multiple times!
        pass

    def _get_missive_template(self):
        return {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
                "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0},
                "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0},
                "face_of_power": self.face_of_power}

    # XXX Perhaps this should be _decision_finalizer? It's not immediately obvious
    # that this needs to be called by decision.
    def _publish_decision(self, decision_packet):
        self.communicator.publish_message(decision_packet)

