RF51 Device Information

Purchasing
The following devices are being used to work on the ble mesh.
nrf51 smart beacon (3) http://www.digikey.com/product-detail/en/NRF51822-BEACON/1490-1036-ND/4811067
nrf51 development board http://www.digikey.com/product-detail/en/NRF51-DK/1490-1038-ND/5022449
tag connect cable http://www.tag-connect.com/TC2030-CTX-NL
nrf51 USB Dongle (optional, use development board instead) http://www.digikey.com/product-detail/en/NRF51-DONGLE/1490-1037-ND/5022448

Overview
Description:https://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF51822
A great resource for developers is here: https://devzone.nordicsemi.com

The simplest thing… 0 to blinky in around 5 minutes.  (Using the mbed SDK with the nrf51)

The mbed platform SDK can be used to develop directly on the nrf51 platform.  This setup uses a fairly beginner friendly open source SDK that provides cross device support for multiple hardware platforms, and doesn’t require any downloads or additional setup!   These steps should work on linux, osx, or windows…

http://evothings.com/getting-started-with-mbed-arms-new-iot-platform-and-the-nordic_semiconductor-nrf51-dk/

Using the Nordic SDK with GCC

While the mbed platform SDK and compiler are great for getting started, and for writing generic cross platform applications for the nrf51, if you plan to work on the mesh network, you will likely need to work directly with the nordic SDK to take advantage of some chip features that are not directly exposed via the mbed SDK.  This is primarily due to the BLE mesh code we are likely going to start with having already been created under the Nordic SDK, and no port for mbed existing (yet).

Setting up a build environment for the Nordic SDK under windows 
This is well covered in the Nordic documentation, and there are plenty of tutorials.  This one is a nice recent one for using eclipse:
https://devzone.nordicsemi.com/tutorials/7/development-with-gcc-and-eclipse/

Setting up with gcc under OSX
This took a bit of piecing together, so to save you time, I have listed the steps below.

Downloads:

get the gcc arm compilation tools.  Install these under /usr/local 
https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q2-update/+download/gcc-arm-none-eabi-4_9-2015q2-20150609-mac.tar.bz2

If you do not have xcode command line utilities installed, you will need to install them.  type gcc in a terminal, you will be prompted to install the command line tools if they have not yet been installed.

      2)   create a local folder (SDK_ROOT in this doc) to house the nordic sdk for the nrf51 (latest is 9.0 here: https://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v9.x.x/ ) 

      3) Now take some time to peruse the docs.  https://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v9.x.x/doc/9.0.0/
if you don’t read anything else, at least read the information about Nordics s110 softdevice.  The Timeslot API is very important for the mesh network code.
http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.pdf.sds/S110_SDS_v2.0.pdf
     4) Grab the jlinkexe binaries for osx and install them.
https://www.segger.com/jlink-software.html
(Both the software and documentation pack, as well as the debugger.)
 
     5) While you are there, get the j-link debugger as well I am sure it will be useful in the future....

Having grabbed these items, you are now ready to set up your environment, which is relatively straightforward in the latest SDK

Edit your GCC toolchain configuration file under SDK_ROOT/components/toolchain/gcc/Makefile.posix and update the settings to look like below:

GNU_INSTALL_ROOT := /usr/local/gcc-arm-none-eabi-4_9-2015q2
GNU_VERSION := 4.9.3
GNU_PREFIX := arm-none-eabi

This will enable you to compile your example code, however the default example code is not configured to target the nrf51 usb dongle.  The Nordic SDK identifies the type of board you are targeting with compilation and Assembler tags that are embedded in the makefiles.  A list of the tags nordic uses can be found here:

http://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v8.x.x/doc/8.0.0/s110/html/a00028.html

Note there are two entries for the usb dongle.  I have had success using BOARD_PCA_10000.  

next, Navigate the blinky example code.

> cd SDK_ROOT/examples/peripheral/blinky

The convention that Nordic has established in this example folder structure is to name the example build files by their associated board identifier.  So  I copied an existing directory over to create a new folder hierarchy for my pca10000.

> cp -r pca10028 pca10000

Now navigate into your new directory.  There are two folders, one for compiling with the s110 softdevice, and one for compiling without the softdevice (blank).  For your first attempt it is easier to load the code directly using the blank example.  Navigate to:

> cd SDK_ROOT/examples/peripheral/blinky/pca10000/blank/armgcc

in this folder edit the Makefile and replace “10028” with “10000”.  This should occur three times in the file.  Once for the output file, once for CFLAGS, and once for ASM flags.  Changing this flag will cause the build to pull in the correct header file for your board, and include the board configuration to map to your code macros for the LED’s.  The changes in this makefile represents the entirety of the difference in the directories between board identifiers.  Once you have made that change, you can simply type make, and if your laptop is like mine and Greg’s, then you should now see an output that looks like this…

$ make
rm -rf _build
echo  Makefile
Makefile
mkdir _build
Compiling file: system_nrf51.c
Compiling file: main.c
Compiling file: nrf_delay.c
Compiling file: gcc_startup_nrf51.s
Linking target: nrf51422_xxac.out
Preparing: nrf51422_xxac.bin
Preparing: nrf51422_xxac.hex

   text	   data	    bss	    dec	    hex	filename
    908	     96	      0	   1004	    3ec	_build/nrf51422_xxac.out

Congratulations, you have built your first binary for the nrf51.  If you failed, I guess give me a call.  Maybe I can remote into your laptop and hotdog it or something.

Now that you have a hex, it would be nice to load it into your chip.  For this we can use jlinkexe on osx.  if you installed the jlink package, it should be in your path already.

(I think the -device option is optional, but I use it)

> jlinkexe -device nrf51822 
> loadfile _build/nrf51422_xxac.hex 
> r
> g

output for me looks like this with my commands typed in red:

Info: Found SWD-DP with ID 0x0BB11477
Info: Found Cortex-M0 r0p0, Little endian.
Info: FPUnit: 4 code (BP) slots and 0 literal slots
Info: CoreSight components:
Info: ROMTbl 0 @ F0000000
Info: ROMTbl 0 [0]: F00FF000, CID: B105100D, PID: 000BB471 ROM Table
Info: ROMTbl 1 @ E00FF000
Info: ROMTbl 1 [0]: FFF0F000, CID: B105E00D, PID: 000BB008 SCS
Info: ROMTbl 1 [1]: FFF02000, CID: B105E00D, PID: 000BB00A DWT
Info: ROMTbl 1 [2]: FFF03000, CID: B105E00D, PID: 000BB00B FPB
Info: ROMTbl 0 [1]: 00002000, CID: B105900D, PID: 000BB9A3 ???
Cortex-M0 identified.
Target interface speed: 100 kHz
J-Link>loadfile _build/nrf51422_xxac.hex 0x0
Halting CPU for downloading file.
Downloading file [_build/nrf51422_xxac.hex]...O.K.
J-Link>r
Reset delay: 0 ms
Reset type NORMAL: Resets core & peripherals via SYSRESETREQ & VECTRESET bit.
J-Link>g

If your board starts blinking red, yellow, white and light and dark blue in sequence, consider yourself set-up.  if you want to try to use the ble version of the code, repeat the steps for replacing the token in the Makefile in the s110 directory (peer to the blank directory).  You should also make sure you have the correct softdevice loaded on your device.  It can be found here:

SDK_ROOT/components/softdevice/s110/hex

Use the same procedure with j-link, but make sure the start addresses match the ones you find in the linker file that is in the directory with your makefile.  For the s110 version you must first loadbin the softdevice at 0x0, and then loadbin your code at 0x18000. Then complete the sequence with r, g.  

Other Stuff...

What about testing?
Have not tried it yet, but this looks promising...

http://www.meldhome.com/2015/03/27/nrfunit-an-embedded-unit-testing-framework-for-nrf-microcontrollers/

mesh code
The mesh code can be found here (https://github.com/NordicSemiconductor/nRF51-ble-bcast-mesh) , along with installation instructions in the project readme.  Note this is for SDK 6 and 7, but the structure for SDK 7 closely resembles that for SDK 9.  I have not tried to build it yet, but I imagine following the steps for SDK 7 should be fairly similar for SDK 9.  I will update this section with any gotcha’s if I encounter them...

Drag and Drop
Alternately, you can also skip the jlink tool and trust the drag and drop feature to load your hex files.  This seems to also work with softdevices.  The nrf51 dongle should appear as a usb drive if plugged into OSX or windows.  A Correctly built hex file can be dropped directly onto the drive, and it should automatically be loaded.  

Firmware over the Air (FOTA)
The RF51822 chip has the ability to update its firmware wirelessly.  Nordic has created a concept of a “softdevice” which essentially is a pre-compiled binary of the bluetooth 4 stack.  If you have an older revision of your chip (apparently pre 1.1), you would need to load t least the S110 softdevice before you could leverage over the air firmware.   The chips we ordered for Atlanta (revision 1.2) arrived pre-configured with this softdevice, so worked directly out of the box for firmware updates. 

Overview of the apps here:
https://www.nordicsemi.com/eng/Products/nRFready-Demo-Apps

It was also nice to see that nordic has made almost everything open and freely available.  For instance, The IoS toolbox app can be built from source found here:
https://github.com/NordicSemiconductor/IOS-nRF-Toolbox


Debugging/Logging
Pretty good details here:
https://devzone.nordicsemi.com/tutorials/6/debugging-with-real-time-terminal/

There also appears to be a wireless logging capability associated with a real time logging tool, but I have not yet spent any time looking into it. 

