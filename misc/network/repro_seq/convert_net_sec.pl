#!/usr/bin/perl
# process input from generated net_secret in kerenl
#   to use as input to our own seq num generator.
#   $ARGV[0] - input file name
#   $ARGV[1] - output file name

use strict;
my $output_string;
my $input_file_name = $ARGV[0];
my $output_file_name = $ARGV[1];
#open input file
open(INPUT_FILE, $input_file_name) or die "Could not open $input_file_name";
#check each line, grab chars
my $line;
my $val;
for $line (<INPUT_FILE>){
    if($line=~m/^.+(\\x[0-9a-f]{2})$/){

	my $val = $1;
	$output_string = $output_string . $val;
    }elsif($line=~m/^.+\\x([0-9a-f]{1})$/){
	#need to pad with a zero - could have avoided this by formatting input
	$val = $1;
	$output_string = $output_string . "\\x0" .  $val;
	print "\n\\x0$val\n";
    }
}
close(INTPUT_FILE);

print $output_string . "\n";

#print to file
open(OUTPUT_FILE, ">$output_file_name") or die "Could not open $input_file_name";
print OUTPUT_FILE $output_string . "\n";
close(OUTPUT_FILE);

 
exit;
