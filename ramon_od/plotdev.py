import ossaudiodev
import struct

class PlotDev(object):
    PARSERS = {
        ossaudiodev.AFMT_U8:     ('=B', 8),
        ossaudiodev.AFMT_S8:     ('=b', 8),
        ossaudiodev.AFMT_S16_LE: ('<h', 16),
        ossaudiodev.AFMT_S16_BE: ('>h', 16),
        ossaudiodev.AFMT_U16_LE: ('<H', 16),
        ossaudiodev.AFMT_U16_BE: ('>H', 16),
    }
    def __init__(self, dev):
        self._format = ossaudiodev.AFMT_U8
        self._channels = 1
        self._speed = 8000
        self._dev = dev

    def setparameters(self, format, channels, rate):
        self._format = format
        self._channels = channels
        self._speed = rate
        self._dev.setparameters(format, channels, rate)

    def write(self, samples):
        self._plot(samples)
    
    def _plot(self, samples):
        raise NotImplementedError

    def _parse(self, buffer):
        fmt, bits = self.PARSERS[self._format]
        return struct.unpack(
            fmt[0] + (fmt[1] * (len(buffer) / (bits / 8))),
            buffer)

    def close(self):
        self._dev.close()

class ConsolePlot(PlotDev):
    def _plot(self, samples):
        for sample in self._parse(samples):
            print " " * int(float(sample) / 128 * 20 - 10), "x"

def open(*args):
    return ConsolePlot(ossaudiodev.open(*args))
