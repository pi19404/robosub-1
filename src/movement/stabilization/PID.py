class PID_Controller:
    def __init__(self, kP, kI, kD, min_integrator, max_integrator, setpoint):
        self.kP = kP
        self.kI = kI
        self.kD = kD

        self.P = 0
        self.I = 0
        self.D = 0

        self.min_integrator = min_integrator
        self.max_integrator = max_integrator
        self.integrator = 0
        self.derivator = 0

        self.setpoint = setpoint

    def compute (self, actual):
        self.error = self.setpoint - actual

        self.P = self.kP * self.error

        self.integrator = self.integrator + self.error
        if self.integrator > self.max_integrator: self.integrator = self.max_integrator
        if self.integrator < self.min_integrator: self.integrator = self.min_integrator
        self.I = self.kI * self.integrator

        self.D = self.kD * (self.error - self.derivator) # self.derivator is, at this point, set to previous error
        self.derivator = self.error

        return self.P + self.I + self.D
    
    def get_P (self):
        return self.P

    def get_I (self):
        return self.I

    def get_D (self):
        return self.D

    def set_setpoint (self, setpoint):
        self.setpoint = setpoint
        self.integrator = 0
        self.derivator = 0
        return
