# raspi_to_stellarium

Sends the ra/dec position of a telescope (wired gpio) to Stellarium (on a PC)

by usage of raspberrypi/wifi or simulates remote connection 

   Build platform: Linux
   
   Hardware: Raspberrypi, PC(simulation)

Prerequisites:

   install wiringPi:

    git clone https://github.com/WiringPi/WiringPi.git

   cd wiringPi

   ./build

   Download raspi_to_stellarium and go to the main directory:
   
   git clone https://github.com/guitarxuser/raspi_to_stellarium.git
   
   cd raspi_to_stellarium

   To setup and execute do following steps:
   
1. Build

   to build the executables please do:
   
   sh build.sh
   
2. Install
   
   sudo sh install.sh


3. Run the server

   cd scripts

   run the dummy_server (remote connection simulation):
   
   sh start_telescope_dummy.sh 

   altenatively run the telescope_server on raspberrypi:

   sh start_telescope.sh

4. Start Stellarium and configure telescope plugin

   for dummy server on local host port 10000

   for raspberrypi on remote host port 10000


5. Stop the server or dummy server

   start second xterm session, go to the raspi_to_stellarium/scripts and do:

   sh stop_telescope.sh

6. Unistall

   sudo sh uninstall.sh


   

   

   

   
