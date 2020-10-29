import subprocess
import matplotlib.pyplot as plt
from scipy.io import wavfile as wav
from scipy.fftpack import fft, fftfreq
import numpy as np
import scipy


forms = ["DF_1", "DF_2", "DF_1_t", "DF_2_t"]
in_fxd_names = ["in_fxd_delta", "in_fxd_step", "in_fxd_level", "in_fxd_sine", "in_fxd_square", "in_fxd_sweep"]

in_names = ["in_delta", "in_step", "in_level", "in_sine", "in_square", "in_sweep"]
out_name = ["out_delta", "out_step", "out_level", "out_sine", "out_square", "out_sweep"]

path     = "C:/Users/Intern/Desktop/TestApp/Debug/TestApp.exe "
in_path  = "C:/Users/Intern/Desktop/TestApp/audio/"
out_path = "C:/Users/Intern/Desktop/tests/Float/"

in_name  = "in_sweep.wav"
out_name = "float_sweep_PEAK_400_12_6"


f_in  = "--in " + in_path + in_name

for f in forms:
	form  = " --form " + str(forms.index(f))
	f_out = " --out " + out_path + out_name + "_" + f + ".wav"
	args = path + f_in + f_out + form
	# print(args)
	subprocess.run(args)

	# rate, data = wav.read(out_path + out_name + str(f) + ".wav")
	# fft_out = fft(data)
	# plt.plot(data, np.abs(fft_out))
	# plt.plot(data[:20000])

