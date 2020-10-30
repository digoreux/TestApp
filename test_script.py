import subprocess
import matplotlib.pyplot as plt
from scipy.io import wavfile as wav
from scipy.fftpack import fft, fftfreq
import numpy as np
import scipy
import itertools
import sys
import json


def set_param(id, value):

	with open(preset_path, 'r') as file:
	    data = json.load(file)
	    p = data["eq_params"]
	    p[id]['val'] = value

	with open(preset_path, 'w') as file:
		json.dump(data, file, indent=2)

	return 0

def single_file(input, output):
	if(sys.argv[1] == "fxd"):
		out_path = fixed_path

	if(sys.argv[1] == "flt"):
		out_path = float_path

	for f in forms:
		form  = " --form " + str(forms.index(f))
		f_in  = "--in " + in_path + input
		f_out = "--out " + out_path + output + "_" + f + ".wav"
		args = path + f_in + f_out + form
		print(args)
		# subprocess.run(args)

def all_files():
	if(sys.argv[1] == "fxd"):
		out_path  = fixed_path
		in_names  = in_fxd_names
		out_names = out_fxd_names

	if(sys.argv[1] == "flt"):
		out_path  = float_path
		in_names  = in_flt_names
		out_names = out_flt_names

	for n in in_names:
		for f in forms:
			form  = " --form " + str(forms.index(f))
			f_in  = "--in " + in_path + n
			f_out = " --out " + out_path + out_names[in_names.index(n)] + "_" + f + ".wav"
			args = path + f_in + f_out + form
			print(args)
# 			# subprocess.run(args)

forms = ["DF_1", "DF_2", "DF_1_t", "DF_2_t"]

in_fxd_names  = ["in_fxd_delta.wav", "in_fxd_step.wav", "in_fxd_level.wav", "in_fxd_noise.wav", "in_fxd_sine.wav", "in_fxd_square.wav", "in_fxd_sweep.wav", "in_fxd_sweep2.wav"]
out_fxd_names = ["fixed_delta", "fixed_step", "fixed_level", "fixed_noise", "fixed_sine", "fixed_square", "fixed_sweep", "fixed_sweep2"]

in_flt_names  = ["in_delta", "in_step", "in_level", "in_sine", "in_square", "in_sweep", "in_noise"]
out_flt_names = ["out_delta", "out_step", "out_level", "out_sine", "out_square", "out_sweep", "out_noise"]

preset_path = "C:/Users/Intern/Desktop/TestApp/Effects/eq_preset.json"
path       = "C:/Users/Intern/Desktop/TestApp/Debug/TestApp.exe "
in_path    = "C:/Users/Intern/Desktop/TestApp/Audio/"
fixed_path = "C:/Users/Intern/Desktop/tests/Fixed/"
float_path = "C:/Users/Intern/Desktop/tests/Float/"

in_name  = "in_fxd_sweep.wav"
out_name = "fixed_sweep"

for i in range(200):
	set_param(0, 200+i*4)
	all_files()



