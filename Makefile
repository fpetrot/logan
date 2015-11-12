.PHONY: aip-all aip-copy aip-gen aip-flash

#===================================================
# Thank's to Adrien Prost-Boucle for the quite complete initial design
#===================================================

aip-all:
	mkdir -p ./romdata
	$(MAKE) aip-copy
	$(MAKE) aip-gen
	$(MAKE) aip-flash

aip-copy:
	cp hwsw/hwsw.runs/impl_1/design_1_wrapper.bit romdata/bitstream.bit
	cp hwsw/hwsw.sdk/myfsbl/Debug/myfsbl.elf romdata/fsbl.elf
	cp hwsw/hwsw.sdk/rb_if_test/Debug/rb_if_test.elf romdata/app.elf

aip-gen:
	cd romdata && bootgen -image boot.bif -w on -o boot.bin

aip-flash:
	program_flash -f romdata/boot.bin -offset 0 -flash_type qspi_single
aip-tty:
	xterm -e "picocom --baud 115200 /dev/ttyUSB1"
