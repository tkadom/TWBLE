:
softdevice=${2-"/Users/tkadom/Nordic/SDK_9.0.0/components/softdevice/s110/hex/s110_softdevice.hex"}
program=${1-"/Users/tkadom/Nordic/nRF51_SDK_8.0.0_5fc2c3a/examples/peripheral/blinky/pca10031/s110/armgcc/_build/nrf51422_xxac.hex"}

echo "$softdevice"
echo "$program"

jlinkexe << END-COMMAND 
device=nrf51822
loadfile $softdevice 0x0
loadfile $program 0x18000
r
g
exit
END-COMMAND
