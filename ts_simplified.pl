#!/usr/local/bin/perl

#---------------------------------------------------------------------
#        start server on local host 
#---------------------------------------------------------------------
#-----------------------
#server->client:
#MessageCurrentPosition (type = 0):#

#LENGTH (2 bytes,integer): length of the message
#TYPE   (2 bytes,integer): 0
#TIME   (8 bytes,integer): current time on the server computer in microseconds
#           since 1970.01.01 UT. Currently unused.
#RA     (4 bytes,unsigned integer): right ascension of the telescope (J2000)
#           a value of 0x100000000 = 0x0 means 24h=0h,
#           a value of 0x80000000 means 12h
#DEC    (4 bytes,signed integer): declination of the telescope (J2000)
#           a value of -0x40000000 means -90degrees,
#           a value of 0x0 means 0degrees,
#           a value of 0x40000000 means 90degrees
#STATUS (4 bytes,signed integer): status of the telescope, currently unused.
#           status=0 means ok, status<0 means some error
#
#
# unsigned 8bit  Integer (Char  -> C)
# unsigned 16bit Integer (Short -> S)
# signed   32bit Integer (Long  -> l)
# unsigned 8bit  Integer (Char  -> C)
# unsigned 64bit Integer (long long  -> Q)
## Little Endian (X86 Order)
# on x86 Processors
#my $out=pack('CSlC',$zahl1,$zahl2,$zahl3,$zahl1);
# on PPC or other not x86 Processors force Little Endian
#my $out=pack('C<S<lC',$zahl1,$zahl2,$zahl3,$zahl1);

# Big Endian (Network Order)
#my $out=pack('C>S>lC',$zahl1,$zahl2,$zahl3,$zahl1);

use strict;
use IO::Socket;
#use Time::Local;
use constant MYPORT => 10000;
my $sock = '';
my $client = 0;

$sock = new IO::Socket::INET(LocalPort => MYPORT,
		             Reuse     => 1,
		             Listen    => 5)
    or die "can't create local socket: $@\n";
print "Accepting connections on Port ", MYPORT, "...\n";

while ($client = $sock->accept()) 
  {
   print "Accepted connection from ",
   $client->peerhost(), ":", $client->peerport(), "\n";  
   my $length=24;
   my $type=0;
   my $time= time();
   my $ra =0xBAAAAB00;
   my $dec=0x20670280;
   my $status=0;
  
   my $out=pack("SSQlll",$length,$type,$time,$ra,$dec,$status);
   my @out_unpacked=unpack('SSQlll',$out);
   $length=$out_unpacked[0];
   $type=$out_unpacked[1];
   $time=$out_unpacked[2];
   $ra=$out_unpacked[3];
   $dec=$out_unpacked[4];
   $status=$out_unpacked[5];
   my $out_unpacked=$length.$type.$time.$ra.$dec.$status;
   print "$out_unpacked\n";  

   while (1) 
    {
    chomp;
    print $client $out;
    print $client $out;
    print $client $out;
   
    }
  $client->close() if defined $client;
  }

