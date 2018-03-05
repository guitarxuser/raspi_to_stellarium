# raspi_to_stellarium

Sends the ra/dec position of a telescope (wired gpio) to stellarium (on a PC)

by usage of raspberrypi/wifi

Prerequisites:

   install wiringPi:

   git clone git://git.drogon.net/wiringPi

   cd wiringPi

   ./build
   
1. Build, install, uninstall 

   to build the executables please do:
   
   sh build.sh
   
   to install:
   
   sudo sh install.sh

   to unistall:

   sudo sh uninstall.sh

2. Run the server

   cd scripts

   run the dummy_server:
   
   sh start_telescope_dummy.sh

   altenatively run the telescope_server on raspberrypi:

   sh start_telescope.sh

3. Start stellarium and configure telescope plugin

   for dummy server on local host port 10000

   for raspberrypi on remote host port 10000


4. Stop the server or dummy server

   start second xterm session, go to the raspi_to_stellarium/scripts and do:

   sh stop_telescope.sh



   

   

   

   