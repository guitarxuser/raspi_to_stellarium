# raspi_to_stellarium

Sends the ra/dec position of a telescope (wired gpio) to stellarium by usage of raspberrypi/wifi

1. First steps:

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

3. Stop the server or dummy server

   start second xterm session, go to the raspi_to_stellarium/scripts directory and do:

   sh stop_telescope.sh

4. Start stellarium and configure telescope plugin

   for dummy server on local host port 10000

   for raspberrypi remote on port 10000


   

   

   

   