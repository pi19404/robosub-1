from abc import ABCMeta, abstractmethod
import math

from base_oligarch import BaseOligarch

class DepthOligarch(BaseOligarch):
    def __init__(self, communicator):
        BaseOligarch.__init__(self, communicator)
        self.depth_threshold = \
                self.communicator.settings["decision"]["depth_threshold"]

    @property
    def face_of_power(self):
        return "DepthOligarch"

    def decision(self, depth_packet, advice_packet):
        missive = self._get_missive_template()

        if not depth_packet:
            return

        if advice_packet and "desired_depth" in advice_packet:
            self.depth_threshold = advice_packet["desired_depth"]

        if depth_packet["DEPTH"] < self.depth_threshold:
            # XXX Better interpret the depth sensor.
            missive['desired_offset']['z'] = -1.0
        else:
            missive['desired_offset']['z'] = 1.0
        self._publish_decision(missive)

