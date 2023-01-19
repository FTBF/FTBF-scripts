#import urllib2 as url #python2
import urllib.request as url #python3
import numpy as np
import sys
from datetime import datetime
import matplotlib.pyplot as plt

# this is static and is the first part of the Acnet URL
acnet_webString = 'http://www-bd.fnal.gov/cgi-bin/acl.pl?acl=logger_get/start='

# Function to construct URL and write results to text file
def get_acnet_data(T1, T2, device):
    # build URL to be queried and decode data to human readable content
    URL = acnet_webString+str(T1)+'/end='+str(T2)+'/node=Swyd+'+str(device)
    #print(URL)
    response = url.urlopen(URL).read()
    decoded_response = response.decode()
    return decoded_response

def parseACNetData(buff):
    time = []
    data = []
    buff_lines= buff.split("\n")
    for line in buff_lines[1:]:
        splitline = line.split()
        if len(splitline) >= 2:
            time.append(datetime.strptime(" ".join(splitline[0:2]), "%d-%b-%Y %H:%M:%S.%f"))#.timestamp())
            data.append([int(float(splitline[i].strip())) for i in (2,)])

    time = np.array(time)
    data = np.array(data)
    return time, data

def return_acnet_data(device, startTime, endTime = None): 
    ## define counters

    if endTime == None:
        endTime = datetime.strftime(datetime.now(), "%d-%b-%Y-%H:%M:%S")

    device_data = get_acnet_data(startTime, endTime, device)

    return parseACNetData(device_data)

def plotBeamSpills(data):
    time, counts = data['F:MT6SC4']
    plt.clf()
    plt.plot(time, np.cumsum(counts > 1000))
    plt.xticks(rotation = 45)
    plt.xlabel("time")
    plt.ylabel("beam spills")
    plt.tight_layout()
    plt.savefig("BeamSpills.png")

def plotParticlesOnTarget(data):
    time, counts = data['F:MT6SC4']
    plt.clf()
    plt.plot(time, np.cumsum(counts))
    plt.xticks(rotation = 45)
    plt.xlabel("time")
    plt.ylabel("counts")
    plt.tight_layout()
    plt.savefig("particlesOnTarget.png")

def saveData(data, fname):
    with open(fname, "w") as f:
        f.write("time, " + ", time, ".join(data.keys())  + "\n")
        for i in range(len(data[list(data.keys())[0]][0])):
            for name, device_data in data.items():
                f.write(", ".join( ["%s, %d"%(datetime.strftime(device_data[0][i], "%d-%b-%Y-%H:%M:%S"), device_data[1][i]) for name, device_data in data.items()] ) + "\n")

if __name__=="__main__":

    devices = ['F:MT6SC1',
               'F:MT6SC2',
               'F:MT6SC3',
               'F:MT6SC4',
               'F:MW1SEM']

    startTime = '09-nov-2022-00:00:00'
    endTime = None

    data = {}

    for device in devices:
        data[device] = return_acnet_data(device, startTime, endTime)

    plotBeamSpills(data)
    plotParticlesOnTarget(data)
    saveData(data, "MTest_counters.csv")
