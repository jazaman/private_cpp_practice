#!/usr/bin/perl

use strict;
use warnings;


sub create_dir {
   
    my ($dir) = @_;
    if(-d "$dir") { # directory exists
        print("$dir already exists copying to existing directory ...\n");
    } elsif ( -e "$dir") { 
        print("$dir name already exists aborting ...\n"); 
        return;
    }

    print("creating directory '$dir' ...\n");
    system("mkdir $dir"); 
}

sub copy_files {
    my ($dir) = @_;
    print("copying files to directory '$dir' ...\n"); 
    system("cp Makefile.generic $dir/Makefile"); 
    system("cp HelloWorld_template.cpp $dir/$dir.cpp"); 
}

foreach my $element (@ARGV) {
    print("[$element] \n");
}

print("Total argument passed is:", ($#ARGV+1), "\n");

for(my $i = 0; $i <= $#ARGV; $i++) {
    printf("$i) $ARGV[$i]\n");
    create_dir($ARGV[$i]);
    copy_files($ARGV[$i]);
}

print "\n";

