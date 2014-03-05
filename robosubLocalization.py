# Exploratory Kalman Filter for Localization
#
# About
# =====
# To understand Kalman filters, I used this amazing website:
#		-> http://greg.czerniak.info/guides/kalman1/
#
# Which way is which?
# ===================
#                        UP    LEFT
#                         z   y
#                         ^  *
#       +------------+    | /
#      /|           /|    |/
#     +-+----------+C|    +-----> x FORWARD
#     | +----------|-+  
#     |/  ROBOSUB> |/
#     +------------+ 
#
# 	'C' is for camera
#  
#
#          z   y
#          ^  *
#          | /
#          |/
# -x <-----+-----> x 
#         /|
#        / |
#       *  v
#     -y  -z
#
# SUB-SPACE
# =========
# (From the perspective of the forward-facing camera)
#  x := Forward  
# -x := Backward
#  y := Left
# -y := Right
#  z := Up
# -z := Down
#
#
# WORLD-SPACE
# ===========
# * Same Orientations as sub-space
# * z is 0.0 exactly at the surface of the water
# * Origin (0,0,0) should be at some corner landmark, so that the pool lies in
#       mostly positive x and positive y space.
#

# numpy is much faster, and easier to use than python's math
import numpy as np
import math
import sys

# Gaussian Function := f()
def f(mu, sigma2, x):
    return 1/sqrt(2.*math.pi*sigma2) * exp(-.5*(x-mu)**2 / sigma2)
    
# Example Sensor Object [ Taken from /src/sensor/sanitation.py ]
#
# sensors = {
#   'gyroscope' : {
#       'gx' : 1,
#       'gy' : 2,
#       'gz' : 3
#   },
#   'accelerometer' : {
#       'ax' : 1,
#       'ay' : 2,
#       'az' : 3
#   },
#   'battery_voltage' : {
#       'voltage' : 5
#   },
#   'depth' : {
#       'value' : 10
#   },
#   'compass' : {
#       'direction' : 0
#   }
# }
    

################################
#                              #
#   Example Filtered Object    #
#                              #
################################
# Pos: meters from origin
# Velocity: meters/second, where North is X-axis
#
# filtered_data = {
#   'global_position' : {
#       'px' : 1,
#       'py' : 2,
#       'pz' : 3
#   },
#   'global_velocity' : {
#       'vx' : 1,
#       'vy' : 2,
#       'vz' : 3
#   },
#   'orientation' : {
#       'theta_x' : 1,
#       'theta_y' : 2,
#       'theta_z' : 3
#   },
# }


################################
#                              #
#      Kalman Filter Math      #
#                              #
################################
    #
    # One-Dimentional Variables
    # -------------------------
    #   x := estimate               
    #   P := Uncertainty Covariance
    #   F := State Transition Matrix
    #   u := motion vector
    #   z := measurement
    #   H := measurement function
    #   R := measurement noise
    #   I := Identity Matrix
    #
    # Predition
    # ---------
    #   x' = F x + u
    #   P' = F * P * F[transpose]
    #
    # Measurement Update
    # ------------------
    #   y = z - H * x
    #   S = H * P * H[transpose] + R
    #   K = P * H[transpose] * S[inverse]
    #   x' = x + (K*y)
    #   P' = (I - K*H)*P

# NOTES
# =====
# * Make sure to disreguard old messages
# * Motion Update
# * Measurement Update
# * Add Update Listener
# * Remove Update Listener
# * Determine timestep // Continuous Function
# * Multivariate guassians -> https://www.youtube.com/watch?v=LXJ5jrvDuEk
# * Markov localization

class Localization(object):
    def __init__(self):
        # beleif "prior"
        # | x  | 
        # | dx |
        # | y  | 
        # | dy |
        # | z  | 
        # | dz |
        self.prior = np.zeros((6,1)) # start at origin, motionless
        
        #initial uncertainty covariance
        self.P = np.identity(6) #ARBITRARY ATM

        # State Transition Matrix := F
		# created with create_state_tranisition_matrix(dt)
        self.F = self.create_state_tranisition_matrix(1.0) # 1.0 seconds
		
		# Since we are factoring in our motion, we must perscribe a 
		#	Control vector 'un'
        self.un = np.zeros((6,1))
        #and a Control Matrix 'B'
        self.B = np.zeros((6,6))
		
        
		# Measurement Function
		#
		#	| 1 0 0 0 0 0 |
        #   | 0 0 0 0 0 0 |
		#	| 0 0 1 0 0 0 |
        #   | 0 0 0 0 0 0 |
		#	| 0 0 0 0 1 0 |
        #   | 0 0 0 0 0 0 |
		#
        self.H = np.matrix([[1,0,0,0,0,0],
                           [0,0,0,0,0,0],
                           [0,0,1,0,0,0],
                           [0,0,0,0,0,0],
                           [0,0,0,0,1,0],
                           [0,0,0,0,0,0]])
        
        # Measurment Noise Matrix 'R'
        self.R = np.eye(6) * 0.15 # ARBITRARY ATM

    def observation_function(state, noise):
        C = np.array([[-1, 0.1], [0.2, 0.1]])
        return np.dot(C, state) + noise
    
    def get_new_prediction(self, dt, prior, orientation0, thrust_vector0):
        # Transiition Matrix
        self.F = self.create_state_tranisition_matrix(self, dt)
        # ***Prior Vector is already defined in self***
        # Control Matrix 'B':
        self.B = create_control_matrix
        # Control Vector 'un':
        self.un = self.create_control_vector(dt)
        # x' = F x + B u
        prior_prime = np.dot(self.F,prior) + np.dot(self.B,self.un)
        return prior_prime #new predition after timestep 'dt'
    
    def get_new_covariance(self):
        #   P' = F * P * F[transpose]
        return np.dot(np.dot(self.F,self.P),np.transpose(self.F))
        
    
    def create_state_tranisition_matrix(self, dt):
		F_stm = np.identity(6)
		F_stm[0][1] = dt # correlating timestep with velocity for each 
		F_stm[2][3] = dt 
		F_stm[4][5] = dt
		return F_stm
		
    def create_control_vector(self, dt): #UNDER CONSTRUCTION
        return np.zeros((6,1)) # 'un'
        # return self.get_fake_control_matrix(dt, control_vector)
        
    def create_control_matrix(self, dt): #UNDER CONSTRUCTION
        return np.zeros((6,6)) # 'B'
		
        
    def measurement_update(self, measurement_pos, observation_matrix):
		# defining variables to keep with equation form
        z = measurement_pos #assuming already in correct form
		H = observation_matrix
		#----Measurement step----
        #   y = z - H * x
        y = z - np.dot(H, self.prior) 
        #   S = H * P * H[transpose] + R
        S = np.dot( np.dot(H, self.P), np.transpose(H) ) + self.R 
        #   K = P * H[transpose] * S[inverse]
        K = np.dot( np.dot(self.P, np.transpose(H)), np.linalg.inv(S) )
		#   x' = x + (K*y)
        prior_prime = self.prior np.dot(K,y)
        #   P' = (I - K*H)*P
        P_prime = np.dot( (np.eye(6)-np.dot(K,H)) , self.P)
        # place new values
        self.prior = prior_prime
        self.P = P_prime
    
    def get_fake_control_matrix(self, dt, control_vector):
        percent_trust = linalg.norm(control_vector)
        normalized_cv = control_vector / linalg.norm(control_vector)
        
        prior = np.array([self.prior[1],self.prior[3],self.prior[5]])
        prior_norm = linalg.norm(prior)
        normalized_prior = prior / prior_norm
        percent_additive = 2. / linalg.norm(normalized_cv + (normalized_prior*(prior_norm/2.))
        
        base_array =  np.array([[0,0,0,0,0,0],
                                [0,1,0,0,0,0],
                                [0,0,0,0,0,0],
                                [0,0,0,1,0,0],
                                [0,0,0,0,0,0],
                                [0,0,0,0,0,1]])
        return base_array * percent_additive
        
        
        
        
    
    
    
        
class SubmarinePhysics():


	#= q*[1-exp(-x*t)] / [1+exp(-x*t)] 
	#####################################################################
	# Website >>  http://my.execpc.com/~culp/rockets/rckt_eqn.html#Method 				

    # kinematic_viscosity_of_water?
    def get_coef_drag(v,kvw):
        pass
        """
        drag coefficient Cd
        pass
    
    def get_drag_force(Cd, rho, Ap, v): 
        '''
        Drag Force = Cd * rho * Ap * v^2 / 2
        Where Cd is the coefficient of drag (calculated using the current velocity and the kinematic viscosity of water)
        rho is the density of water
        Ap is the projected area of the sub (otherwise known as the cross sectional area)
        v is the velocity (relative to the surrounding fluid)
        '''
        return ( Cd * rho * Ap * (v*v) )/ 2.
        
        
def c(self, dt):
    F_stm = np.identity(6)
    F_stm[0][1] = dt # correlating timestep with velocity for each 
    F_stm[2][3] = dt #  axis
    F_stm[4][5] = dt
    return F_stm


