#!/usr/local/bin/perl

use strict;
use IO::Socket;
use Time::HiRes qw (sleep);
#use Time::Local;
use constant MYPORT => 10000;
my $sock = '';
my $client = 0;
my $REVOL_STEPS_COUNT=96;
my $MAX_NUMBER=4294967296;
my $globalCounter=0;
my @devices;
my $index=0;
#---------------------------------------------------------------------
#        start processing
#---------------------------------------------------------------------

if (-e "/tmp/devices.txt")
  {
   open (LOGFILE,"< /tmp/devices.txt") or die "could not open logfile : $! \n"  ;
  
   foreach    my $line  (<LOGFILE>)
    {
     print "$line\n" ;   
     @devices=split(';',$line);
    }
#   print   @devices;  
  }
close(LOGFILE);
#synchronisation

my $serial=$devices[0];
open (TTY, ">$serial")  or die $!;
    while (1) {
	
   sleep(0.02);
    
   #calculate ra

   my  $ra_calc=($globalCounter/$REVOL_STEPS_COUNT)*$MAX_NUMBER;
       $ra_calc= sprintf("%08X", $ra_calc);
       print("counter is $globalCounter $ra_calc\n");
       chomp;
#   while($index<10)
#   {
       print  TTY "$ra_calc,5B000000\r\n";
#       print  TTY "$ra_calc,5B000000\r\n";
#       print  TTY "$ra_calc,5B000000\r\n";
#       print  TTY "$ra_calc,5B000000\r\n";
#       print  TTY "$ra_calc,5B000000\r\n";
#       print  TTY "$ra_calc,5B000000\r\n";
#       print  TTY "$ra_calc,5B000000\r\n";
#       print  TTY "$ra_calc,5B000000\r\n";
#       $index++
 #  }
       $globalCounter++;
       if ($globalCounter==$REVOL_STEPS_COUNT)
        {
	 $globalCounter=0;
        }   
}
 
   

