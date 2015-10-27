#wc -c mbr.bin == 320
cp mbr.bin ndisk.img
dd if=./ddisk.img iflag=skip_bytes skip=320 of=./ndisk.img oflag=append conv=notrunc

