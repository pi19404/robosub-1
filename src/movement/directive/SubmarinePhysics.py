import numpy as np
from numpy import linalg
import math

class SubmarinePhysics():
    """ THis class is meant for calculating variables that otherwise 
    would be tedious and complicated to calculate.  In this class, the 
    methods should be very simple."""

    
    """Most of these equations are similar to rocketry equations;
    as in ;> MAX Velocity @ burnout = q*[1-exp(-x*t)] / [1+exp(-x*t)]
    
    For us, we are interested in the aswer to this question over 
    3 dimentions and a very small amount of time."""
    
    def __init__(self):
        #'CONSTANT' VALUES
        self.MASS = 65.0 * 0.453592 #mass in kg
        self.DENSITY_OF_WATER = 1000.0 #kg/m^3
        self.DRAG_COEFFICIENT = 1.45 # IDK how to find this. estimated from wikipedia
        self.MOTORCONFIG = []
        self.MAX_THRUST = 3.254 # Newton meters
        self.THRUSTERS = []
        self.VISCOSITY_OF_WATER = 1.004 # (m^2/s) x 10^-6
        self.SUB_DIMENSIONS = [1.0, 0.4, 0.4] # l, w, h (meters)
        # self.BOUYANCY_Z_FORCE = ??????
        self.configThrusters()
    
    #@staticmethod
    def getThrust(self, orderedMotorThurstPercentages,newMotorConfiguration=False):
        # OLD MOTOR CONFIGURATION
        # Motors A,C,D,F thrust normally "forward"
        # Motors B,E thrust up and inward
        #            (y)
        #            BOW
        #       +--_----_---+
        #       | /A/   \F\ |     
        #       |/_/     \_\|
        #       +---+~~~+--+
        #       |   | S |   |
        #       |   | U |   |
        #       |__ | B | __|
        # PORT  (_B()   ()E_) STARBOARD  (x)
        #       |   |   |   |
        #       |   |   |   |
        #       |   |   |   |
        #       +-_-+~~~+-_-+        ( z is up ) 
        #       | \C\   /D/ |
        #       |  \_\ /_/  |
        #       +-----------+
        #           STERN
            
        #THRUSTERS = [ a, b, c, d, e ,f ]
        thrustVector = np.array([0,0,0])
        for i in range(len(self.THRUSTERS)):
            thrustVector = thrustVector + (self.THRUSTERS[i] * orderedMotorThurstPercentages[i]*self.MAX_THRUST)
        return thrustVector #TODO: factor in bouyancy?
    
    
    def configThrusters(self,newMotorConfiguration=False):
        if(newMotorConfiguration):#2014 motor config
            self.THRUSTERS =  np.array([0,0,0])
        else: # 2013's Motor Configuration
            # defined below are the general angle of the thrusters,
            #   along with a normalizing action
            a = np.array([1,1,0])
            a = a / linalg.norm(a)
            
            b = np.array([1,0,1])
            b = b / linalg.norm(b)
            
            c = np.array([-1,1,0])
            c = c / linalg.norm(c)
            
            d = np.array([1,1,0])
            d = d / linalg.norm(d)
            
            e = np.array([-1,0,1])
            e = e / linalg.norm(e)
            
            f = np.array([-1,1,0])
            f = f / linalg.norm(f)
            
            self.THRUSTERS = [ a, b, c, d, e ,f ]
    

    
    def getSurfaceArea(self,normalVector):
        # cylinders (battery hull, main hull)
        # 6 Bars on length,
        # 6 bars on width
        # 4 bars on verticle
        # 6 Thrusters
        
        # TODO: finalize this function to actually find our value
        # SIMPLIFIED HACK: Rectangular Prism Only
        return 1.0*0.4 + 0.4*0.4 # square meters ~
    
    def getLittleK(self, normalVector):
        '''
        Drag Force = Cd * rho * Ap * v^2 / 2
        Where Cd is the coefficient of drag (calculated using the current velocity and the kinematic viscosity of water)
        rho is the density of water
        Ap is the projected area of the sub (otherwise known as the cross sectional area)
        v is the velocity (relative to the surrounding fluid)
        k = 0.5*rho*Cd*A = 0.5*1.2*0.75*A
        '''
        return 0.5 * self.VISCOSITY_OF_WATER * self.DRAG_COEFFICIENT * self.getSurfaceArea(normalVector)
        
    def getLittleQ(self, thrustVector, normalVector):
        thr = linalg.norm(thrustVector)
        k = self.getLittleK(normalVector)
        q = math.sqrt( (thr / k) ) # q = sqrt([T - M*g] / k) HOWEVER there is no gravity
        return q
        
    
    def getVelocity(self, dt, normalVector, orderedMotorThurstPercentages):
        # v = q*[1-exp(-x*t)] / [1+exp(-x*t)]
        thrust = self.getThrust(orderedMotorThurstPercentages)
        
        q = self.getLittleQ(thrust, normalVector)
        k = self.getLittleK(normalVector)
        x = 2*k*q / self.MASS
        v =  q * (1-math.exp(-x*dt)) / (1+math.exp(-x*dt))
        return v
