#!/usr/local/bin/perl

#---------------------------------------------------------------------
#        start processing
#---------------------------------------------------------------------

$cmd_line="sh $ENV{'PWD'}/tty0tty.sh" ;

print "cmd_line $cmd_line \n";

open(IN_TTY,"$cmd_line|") or die "could not start tty0tty : $!";



foreach   my $line_tty  (<IN_TTY>)
{

        print "tty line $line_tty\n";
    			
}

#close (IN_TTY);

exit(0);
