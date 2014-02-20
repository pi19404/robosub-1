# Kalman Filter

import json
import zmq
import os
import math
import numpy as np

kalman = {}

def config(values=[], error_cov=1, kalman_value_prev=0, std_dev=0.1):
	kalman['values'] = values
	kalman['error_cov'] = error_cov
	kalman['kalman_value_prev'] = kalman_value_prev
	kalman['n_step'] = len(values)

	kalman['std_dev'] = np.std(kalman['values'])



	print "Data set: ", kalman['values']
	print "Err Cov: ", error_cov 
	print "Kalman Value Init:", kalman_value_prev 
	print "Std Dev:", kalman['std_dev'] 
	print

def main():
	print ("Kalman Filter....")

def prediction():
	kalman['kalman_value_predict'] = kalman['kalman_value_prev']
	kalman['error_cov_predict'] = kalman['error_cov']
	print "----------- Prediction -------------"
	print "Kal Value Predict:", kalman['kalman_value_predict']
	print "Err Cov Predict:", kalman['error_cov_predict']
	print

def correction(index):
	print "----------- Correction -------------"
	kalman['kalman_gain'] = kalman['error_cov_predict'] / (kalman['error_cov_predict'] + kalman['std_dev'])
	kalman['kalman_value'] = kalman['kalman_value_predict'] + kalman['kalman_gain']*(kalman['values'][index] - kalman['kalman_value_predict'])
	kalman['kalman_value_prev'] = kalman['kalman_value']

	kalman['error_cov'] = (1 - kalman['kalman_gain'])*kalman['error_cov_predict']
	print "Kal Gain", kalman['kalman_gain']
	print "Kal Value", kalman['kalman_value']
	print "Err Cov", kalman['error_cov']
	print

def run():
	for ind in range(kalman['n_step']):
		prediction()
		correction(ind)
		print "Kalman[", ind, "]=" , kalman['kalman_value']
		print





if __name__ == "__main__":
	main()