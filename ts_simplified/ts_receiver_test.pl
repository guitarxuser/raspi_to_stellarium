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
    
#---------------------------------------------------------------------
#        start processing
#---------------------------------------------------------------------

$sock = new IO::Socket::INET(LocalPort => MYPORT,
			     Type      => SOCK_STREAM,
		             Reuse     => 1,
		             Listen    => 5)
    or die "can't create local socket: $@\n";
print "Accepting connections on Port ", MYPORT, "...\n";

while ($client = $sock->accept()) 
  {
   print "Accepted connection from ",
       $client->peerhost(), ":", $client->peerport(), "\n";
   while(1){
   my $data;
   # read operation on the newly accepted client
#   $data = <$sock>;
# we can also read from socket through recv()  in IO::Socket::INET
   $client->recv($data,160);
   my @out_unpacked=unpack('SSQLll',$data);
   my $length=$out_unpacked[0];
   my $type=$out_unpacked[1];
   my $time=$out_unpacked[2];
   my $ra=$out_unpacked[3];
   my $dec=$out_unpacked[4];
   my $status=$out_unpacked[5];
   my $out_unpacked=$length.$type.$time.$ra.$dec.$status;
   print "Received from Client : $out_unpacked\n";
   my $ra_hex=sprintf("%08X",$ra);
   my $dec_hex=sprintf("%08X",$dec);
   print "ra=$ra         dec=$dec\n";
   print "ra=$ra_hex          dec=$dec_hex\n";
   my $out_calc=ra_dec_time($ra,$dec); 
   for (my $i=0; $i<2; $i++)
     { 
       print $client $out_calc;
     }
    }
  }      
  
    $client->close() if defined $client;

sub ra_dec_time()
{
   my $length=24;
   my $type=0;
   my $time= time();
#   my $ra =0xBAAAAB00;
   my $ra_inp = shift(@_);
   my $dec_inp= shift(@_);
   my $status=0;
   my $out=pack("SSQLll",$length,$type,$time,$ra_inp,$dec_inp,$status);
  
   return $out;     
}
