import math
import ossaudiodev
import cairoplot

FORMAT = ossaudiodev.AFMT_S16_LE
CHANNELS = 2
RATE = 22050

oss = ossaudiodev.open("/dev/dsp", 'w')
cairo = cairoplot.CairoPlot(oss)
cairo.setparameters(FORMAT, CHANNELS, RATE)

try:
    file = open("audiodump.pcm", "rb")
    while True:
        buffer = file.read(1024 * 16)
        if not buffer:
            break
        oss.write(buffer)
        cairo.write(buffer)
finally:
    oss.close()
