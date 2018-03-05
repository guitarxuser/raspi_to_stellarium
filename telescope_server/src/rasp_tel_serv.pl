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
$serial=$devices[1];

$cmd_line2="sh $ENV{'PWD'}/rasp_tel_serv.sh 10000 $serial";

#$cmd_line="\./TelescopeServerNexStar 10000 $serial " ;

print "cmd_line2 $cmd_line2 \n";

open(IN,"$cmd_line2|") or die "could not start rasp_tel_serv.sh: $!";



foreach   my $line  (<IN>)
{

        print "telescope line $line\n";
    			
}
 
#close(IN);
exit(0);
