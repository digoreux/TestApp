TestApp

Chain parameters are set in Effect/chain_preset.json

--in  <path-to-file>

--out <path-to-file>

--gen <sample_rate>,<time(msec)>,<type>:[parameters] - Generate specific signal <type>
                                                       in IEEE 754 format
                                                       with coresponding [parameters]

-f  generate in PCM format.
-e  apply effect to generated signal.

Generator type and coresponding parameters:

<delta> - Dirak delta function

<step>  - Step function 

<noise>:[amp] - White noise, amplitude in dB.

<square>:[period(ms),amp] - Square signal, period in ms, amplitude in dB. 

<sine>:[frequency,amp] - Sine signal, frequency in Hz, amplitude in dB.

<chirp_log>:[start_frequency,end_frequency,amp] - Sine logarithmic frequency sweep, 
                                                  frequency start/end in Hz, 
                                                  amplitude in dB.

<chirp_lin>:[start_frequency,end_frequency,amp] - Sine linear frequency sweep, 
                                                  frequency start/end in Hz, 
                                                  amplitude in dB.

<sweep>:[frequency, start_db, end_db] - Sine linear level sweep, 
                                        frequency in Hz, 
                                        amplitude start/end in dB.
