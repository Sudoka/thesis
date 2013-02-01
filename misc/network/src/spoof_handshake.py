#!/usr/bin/python

import sys
from scapy.all import TCP, IP
import subprocess


print("Hello!\n")

#initialize command line arguments (TODO: need to sanitize input and automate ip grab)
local_ip = sys.argv[1]
local_port = sys.argv[2]
remote_ip = sys.argv[3]
remote_port = sys.argv[4]
spoof_ip = sys.argv[5]
spoof_port = sys.argv[6]

#determine sequence numbers ahead-of-time to reduce delay before spoof
#local_ip here is dest_ip for server
seq_num_output_current = subprocess.check_output(["repro_seq/seq_create", remote_ip, remote_port, local_ip, local_port])

seq_num_output_spoof = subprocess.check_output(["repro_seq/seq_create", remote_ip, remote_port, spoof_ip, spoof_port])

print(seq_num_output_current)
print(seq_num_output_spoof)

#create connection to server and record sequence number

leg_outbound_ip = IP(dst=remote_ip, src=local_ip)
leg_outbound_tcp = TCP(sport=local_port, dport=remote_port)
packet = leg_outbound_ip/leg_outbound_tcp


#calculate time from sequence number

#spoof!

