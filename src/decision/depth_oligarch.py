from abc import ABCMeta, abstractmethod
import math

from base_oligarch import BaseOligarch

class DepthOligarch(BaseOligarch):
    def __init__(self, communicator):
        BaseOligarch.__init__(self, communicator)
        self.face_of_power = "PathOligarch"
        self.depth_threshold = \
                self.communicator.settings["decision"]["depth_threshold"]

    def decision(self, depth_packet):
        missive = self._get_missive_template()

        if not depth_packet:
            return

        if depth_packet["DEPTH"] < self.depth_threshold:
            # XXX Better interpret the depth sensor.
            missive['desired_offset']['z'] = -1.0
        self._publish_decision(missive)

