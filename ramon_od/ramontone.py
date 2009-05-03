import math
import struct
import ossaudiodev
import cairoplot

FORMAT = ossaudiodev.AFMT_U8
CHANNELS = 1
RATE = 22050

oss = ossaudiodev.open("/dev/dsp", 'w')
cairo = cairoplot.CairoPlot(oss)
cairo.setparameters(FORMAT, CHANNELS, RATE)

def dx_sin(x, zero, height):
    return math.sin(x) / math.pi * height + zero

def freq(x, freq, zero, var):
    return dx_sin(float(x) / RATE * freq, zero, var)

def raw(x):
    return chr(int(x))

try:
    for x in xrange(5 * RATE * CHANNELS):
        buffer = raw(freq(x, 1000, 128, 128))
        oss.write(buffer)
        cairo.write(buffer)
finally:
    oss.close()
