#!/usr/bin/perl

use strict;
use warnings;

# user-defineable variables
my $ALPHABET_LENGTH = 4;
my $MIN_ALL_POSSIBLE_STRING_LENGTH = 1;
my $MAX_ALL_POSSIBLE_STRING_LENGTH = 5;
my $RANDOM_STRINGS_TO_TEST = 500;
my $MIN_RAND_STRING_LENGTH = $MAX_ALL_POSSIBLE_STRING_LENGTH + 1;
my $MAX_RAND_STRING_LENGTH = 5000;
my $RANDOM_STRING_ALPHABET = 26;
my $MIN_PATTERN_SIZE = 2;
my $MAX_PATTERN_SIZE = 100;
my $MIN_COPY = 2;
my $MAX_COPY = 20;

# these constants should remain unchaged
my $ascii_start = 97;

for (my $j = $MIN_ALL_POSSIBLE_STRING_LENGTH; $j <= $MAX_ALL_POSSIBLE_STRING_LENGTH; $j++) {
	my $alphabet = '{' . join(',', chr($ascii_start) .. chr($ascii_start + $ALPHABET_LENGTH - 1)) . '}';

	# glob '{a,b,c,d}' x 5 generates all possible strings of length 5
	# with the alphabet of {a,b,c,d}
	my @a = glob $alphabet x $j;
	my $array_length = @a;

	print "Testing all possible strings (n = $array_length) of length $j from alphabet of length $ALPHABET_LENGTH...\n";
	print((test_array(\@a)) ? "OK\n" : "Failed\n");
}

print "Testing $RANDOM_STRINGS_TO_TEST random strings of length $MIN_RAND_STRING_LENGTH-$MAX_RAND_STRING_LENGTH with alphabet of length $RANDOM_STRING_ALPHABET\n";
my @random;
fill_random_string_array(\@random, $RANDOM_STRINGS_TO_TEST, $MIN_RAND_STRING_LENGTH, $MAX_RAND_STRING_LENGTH, $RANDOM_STRING_ALPHABET);
print((test_array(\@random)) ? "OK\n" : "Failed\n");

print "Testing $RANDOM_STRINGS_TO_TEST cyclic random strings with alphabet of length $RANDOM_STRING_ALPHABET and pattern sizes $MIN_PATTERN_SIZE-$MAX_PATTERN_SIZE with copy number $MIN_COPY-$MAX_COPY\n";
@random = ();
my @patterns; 
fill_random_string_array(\@patterns, $RANDOM_STRINGS_TO_TEST, $MIN_PATTERN_SIZE, $MAX_PATTERN_SIZE, $RANDOM_STRING_ALPHABET);
my @copy_num = $MIN_COPY .. $MAX_COPY;
@random = map { $_ x $copy_num[rand @copy_num] } @patterns;
print((test_array(\@random)) ? "OK\n" : "Failed\n");

sub fill_random_string_array
{
	my ($a, $array_length, $min_string_length, $max_string_length, $alphabet_size) = @_;
	my @chars = chr($ascii_start) .. chr($ascii_start + $alphabet_size - 1);
	my @string_lengths = $min_string_length..$max_string_length;
	for (my $j = 0; $j < $array_length; $j++) {
		my $tmp;
		my $current_string_length = rand @string_lengths;
		for (my $i = 0; $i < $current_string_length; $i++) {
			$tmp .= $chars[rand @chars];
		}
		push @$a, $tmp;
	}
}

sub test_array
{
	my $a = shift;
	my $return_value = 1;
	foreach my $A (@$a) {
		my $len = length($A);
		my %B;
		my $A_old = $A;
		for (my $i = $len; $i > 0; $i--) {
			$A = chop($A) . $A;
			$B{$A} = $i;
		}
		my @sorted = sort keys %B; 
		my $first_key = $sorted[0];
		my $first_value = $B{$sorted[0]};
		my $result = `./canon $A_old`;
		#print "$A_old\t$result";
		chomp $result;
		my ($msp, $d, $str_out) = split("\t", $result);
		if ($first_key ne $str_out) {
			print "Rotation failed: returned $str_out but was expecting $first_key\n";
			exit(1);
		}
		my $exit_code = `echo $?`;
		chop $exit_code;	# remove the trailing new line character
		if ($exit_code != 0) {
			print "Program returned exit code $exit_code for input string $A_old\n";
			exit(1);
		}
		#print "canon reported MSP($A_old) = $msp, d = $d\n";
		if ($msp != $first_value) {
			my %rotations;
			$A = $A_old;
			for (my $i = $len; $i > 0; $i--) {
				$A = chop($A) . $A;
				$rotations{$i} = $A;
			}
			if ($rotations{$msp} ne $first_key) {
				print "Expected MSP($A_old) = $first_value but $msp was returned (d = $d)\n";
				exit(1);
			}
		}
	}
	return $return_value;
}

0;
