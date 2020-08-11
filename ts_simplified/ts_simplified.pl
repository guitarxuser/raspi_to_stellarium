#!/usr/local/bin/perl
use strict;
use IO::Socket;
use Time::HiRes qw (sleep);
#use Time::Local;
use constant MYPORT => 10000;
my $sock = '';
my $client = 0;
my $REVOL_STEPS_COUNT=256;
my $MAX_NUMBER=4294967296;
my $globalCounter=0;
my @devices;
my $ra_last;
my $dec_last;
my $ra_changed="true";
my $dec_changed="true";    
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
my $serial=$devices[1];

$sock = new IO::Socket::INET(LocalPort => MYPORT,
                             Type      => SOCK_STREAM,
		             Reuse     => 1,
		             Listen    => 5)
    or die "can't create local socket: $@\n";
print "Accepting connections on Port ", MYPORT, "...\n";

#open(IN,"$cmd_line|") or die "could not start $cmd_line: $!";
open (my $tty, $serial)  or die $!;

while ($client = $sock->accept()) 
  {
   print "Accepted connection from ",
   $client->peerhost(), ":", $client->peerport(), "\n";
   my $line;


      while(1)
      {
#        sleep(0.02);  
        $line=<$tty>; 
        print "tty line $line\n";
        my $out_calc=ra_dec_time($line);
#        sleep(0.5); 
        print $client $out_calc;

     }
  }
   


   
  
    $client->close() if defined $client;

sub ra_dec_time()
{
   my $length=24;
   my $type=0;
   my $time= time();
#   my $ra =0xBAAAAB00;
   my $ra_dec_inp = shift(@_);
   my @ra_dec=split(',',$ra_dec_inp);
   my $ra = hex("$ra_dec[0]");
   my $dec= hex("$ra_dec[1]"); 
#   my $dec=543621760;

   my $status=0;

   my $out=pack("SSQLll",$length,$type,$time,$ra,$dec,$status);
   my @out_unpacked=unpack('SSQLll',$out);
   $length=$out_unpacked[0];
   $type=$out_unpacked[1];
   $time=$out_unpacked[2];
   $ra=$out_unpacked[3];
   $dec=$out_unpacked[4];
   $status=$out_unpacked[5];
   my $out_unpacked=$length.$type.$time.$ra.$dec.$status;
   print("time=$time    ra=$ra    dec=$dec\n");
   print "$out_unpacked\n";
   return $out;     
}
