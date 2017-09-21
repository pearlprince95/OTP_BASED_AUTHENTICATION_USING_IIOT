# OTP_BASED_AUTHENTICATION_USING_IIOT
An OTP based door unlocking system, It sends sms to your registered number through Twilio online sms client.

# PRE-REQUISITIES
python2.7
Install pip if you are using windows computer
if raspberry pi:
pip install twilio
pip install paho-mqtt

# IF YOY WANT TO RUN YOUR SCRIPT BACKGROUND IN RPI 

# Creating your python script
My example script was stored in the /home/pi directory and named “OTP_BASED_DOOR_UNLOCKING_SYSTEM.py”. Obviously your script can be called something else but keep an eye on where it is referenced in the commands and text below.

# Create A Unit File
Next we will create a configuration file (aka a unit file) that tells systemd what we want it to do and when :
sudo nano /lib/systemd/system/OTP_BASED_DOOR_UNLOCKING_SYSTEM.service

Add in the following text :
[Unit]
Description=OTP_BASED_DOOR_UNLOCKING_SYSTEM Service
After=multi-user.target
[Service]
Type=idle
ExecStart=/usr/bin/python /home/pi/OTP_BASED_DOOR_UNLOCKING_SYSTEM.py
[Install]
WantedBy=multi-user.target

You can save and exit the nano editor using [CTRL-X], [Y] then [ENTER].

This defines a new service called “OTP_BASED_DOOR_UNLOCKING_SYSTEM Service” and we are requesting that it is launched once the multi-user environment is available. The “ExecStart” parameter is used to specify the command we want to run. The “Type” is set to “idle” ensures the ExecStart command is only run when everything else has loaded. For my GPIO based scripts the default type of “simple” didn’t work.

Note that the paths are absolute and fully define the location of Python as well as the location of our Python script.

In order to store the script’s text output in a log file you can change the ExecStart line to :
ExecStart=/usr/bin/python /home/pi/OTP_BASED_DOOR_UNLOCKING_SYSTEM.py > /home/pi/OTP_BASED_DOOR_UNLOCKING_SYSTEM.log 2>&1

The permission on the unit file needs to be set to 644 :
sudo chmod 644 /lib/systemd/system/OTP_BASED_DOOR_UNLOCKING_SYSTEM.service

# Configure systemd
Now the unit file has been defined we can tell systemd to start it during the boot sequence :
sudo systemctl daemon-reload
sudo systemctl enable OTP_BASED_DOOR_UNLOCKING_SYSTEM.service

sudo reboot

# Check status of your service
sudo systemctl status OTP_BASED_DOOR_UNLOCKING_SYSTEM.service
