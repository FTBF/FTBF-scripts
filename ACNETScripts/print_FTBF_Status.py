#!/usr/bin/env python3
'''
A sample script to reproduce the values in the FTBF_Status - Synoptic Viewer.
Feel free to use the code snippets.
'''
__author__ = 'Sangbaek Lee, Whitney Armstrong'

import subprocess

class pyACNET():
  def __init__(self):
    self.polarities = {'Positive': +1.0, 'Negative': -1.0}
    self.talking = None
    self.read_values()
    if self.talking:
      self.print_values()
 
  def read_values(self):
    """
    to read beamline vars from ACNET.
    """
    try:
      self.sctime = float(subprocess.run(["ssh", "ftbf-daq-08","wget","-q","-O","-",
        "http://www-bd.fnal.gov/cgi-bin/acl.pl?read+G:SCTIME"],
        stdout=subprocess.PIPE).stdout.decode("utf-8").split()[-2])
      self.MTSCL5 = float(subprocess.run(["ssh", "ftbf-daq-08","wget","-q","-O","-",
        "http://www-bd.fnal.gov/cgi-bin/acl.pl?read+F:MTSCL5"],
        stdout=subprocess.PIPE).stdout.decode("utf-8").split()[-2])
      MT6VT2 = float(subprocess.run(["ssh", "ftbf-daq-08","wget","-q","-O","-",
        "http://www-bd.fnal.gov/cgi-bin/acl.pl?read/setting+F:MT5VT2"],
        stdout=subprocess.PIPE).stdout.decode("utf-8").split()[-2])
      MT6VT2_polarity = str(subprocess.run(["ssh", "ftbf-daq-08","wget","-q","-O","-",
        "http://www-bd.fnal.gov/cgi-bin/acl.pl?read/polarity+F:MT5VT2"],
        stdout=subprocess.PIPE).stdout.decode("utf-8").split()[-1])
      self.MT6VT2 = self.polarities[MT6VT2_polarity] * MT6VT2
      MT6HT2 = float(subprocess.run(["ssh", "ftbf-daq-08","wget","-q","-O","-",
        "http://www-bd.fnal.gov/cgi-bin/acl.pl?read/setting+F:MT5HT2"],
        stdout=subprocess.PIPE).stdout.decode("utf-8").split()[-2])
      MT6HT2_polarity = str(subprocess.run(["ssh", "ftbf-daq-08","wget","-q","-O","-",
        "http://www-bd.fnal.gov/cgi-bin/acl.pl?read/polarity+F:MT5HT2"],
        stdout=subprocess.PIPE).stdout.decode("utf-8").split()[-1])
      self.MT6HT2 = self.polarities[MT6HT2_polarity] * MT6HT2
      self.talking = True
    except Exception as e:
      print(e)
      print("debug:: Renew the kerberos.")

  def print_values(self):
    spill_length = 4.2
    self.nextspilltime = (60 - self.sctime + spill_length)%60
    print("SC1+SC2+SC3+Spill (F:MTSCL5)\t{:>3.0f} Cnts".format(self.MTSCL5))
    print("Next Spill\t{:>20.0f} Sec".format(self.nextspilltime))
    print("G:SCTIME\t{:>20.0f} Sec".format(self.sctime))
    print("F:MT5VT2\t{:>19.0f} Amps".format(self.MT6VT2))
    print("F:MT5HT2\t{:>19.0f} Amps".format(self.MT6HT2))

def main():
  __acnet = pyACNET()

if __name__ == "__main__":
  main()
