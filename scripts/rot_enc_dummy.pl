#!/usr/local/bin/perl

#---------------------------------------------------------------------
#        start processing
#---------------------------------------------------------------------

if (-e "/tmp/devices.txt")
  {
   open (LOGFILE,"< /tmp/devices.txt") or die "could not open logfile : $! \n"  ;
  
   foreach    $line  (<LOGFILE>)
    {
     print "$line\n" ;   
    @devices=split(';',$line);
    }
#   print   @devices;  
}
close(LOGFILE);
#synchronisation
$serial=$devices[0];

$cmd_line3="sh $ENV{'PWD'}/rot_enc_dummy.sh $serial";


print "cmd_line3 $cmd_line3 \n";

open(IN,"$cmd_line3|") or die "could not start rot_enc.sh: $!";



foreach   my $line  (<IN>)
{

        print "telescope line $line\n";
    			
}
 
#close(IN);
exit(0);
