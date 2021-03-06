#!/usr/bin/perl
# clip_rand_read_dmesg.pl - a simple regex script which cuts out the prelimary 
#   portion of the DEBUG_ENTROPY statements from random.c so that only the 
#   rand_read_num portion remains.  This is used in comparing across boots with
#   slightly different timings recorded by the kernel.
# Args:
#   $ARGV[0] - input filename
#   $ARGV[1] - output filename
# Author: Dan Cashman
#
use strict;

my $input_filename = $ARGV[0];
my $output_filename = $ARGV[1];
my @output_lines;
my $count = 0;
open (INPUT_FILE, $input_filename) or die "Could not open $input_filename\n";
#iterate through lines
for my $line (<INPUT_FILE>){
    #regex stuff 
    $line =~ s/^.+(rand_read_num.*$)/$1/;
    $output_lines[$count++] = $line;
}
close INPUT_FILE;

open (OUTPUT_FILE, ">$output_filename") or die "Could not open $output_filename\n";
foreach (@output_lines){
    print OUTPUT_FILE $_;
}
close OUTPUT_FILE;
