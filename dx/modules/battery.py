#!/usr/bin/python

FILENAME_INFO = "/proc/acpi/battery/BAT1/info"
FILENAME_STAT = "/proc/acpi/battery/BAT1/state"
#BUG_MASK = 0b0000111111111111 # 0x0fff
BUG_MASK =  0b0001111111111111 # 0x1fff

class Battery(object):
    def __init__(self):

        stat = self.parse(FILENAME_STAT)
        info = self.parse(FILENAME_INFO)

        self.present = stat['present']
        self.capacity_state = stat['capacity state']
        self.charging_state = stat['charging state']
        self.rate = int(stat['present rate'].split()[0]) / 1000.0
        self._remaining_orig = int(stat['remaining capacity'].split()[0])
        self._voltage_orig = int(stat['present voltage'].split()[0])
        self._full_orig = int(info['last full capacity'].split()[0])
        
        self.remaining = self.apply_mask(self._remaining_orig) / 1000.0
        self.full = self.apply_mask(self._full_orig) / 1000.0
        self.voltage = self._voltage_orig / 1000.0
    
    def parse(self, filename):
        return dict([
            tuple([token.strip() for token in line.split(":")])
            for line in open(filename).read().strip().split("\n")
            if line.count(":")])

    def apply_mask(self, value):
        if value > 0x1000:
            return value & BUG_MASK
        else:
            return value

def main():
    bat = Battery()
    print bat.remaining, "Ah"
    print round(bat.remaining / bat.full * 100, 1), "%"
