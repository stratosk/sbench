CC=gcc

all: sbench calibrate

sbench:
	$(CC) sbench.c -o sbench

calibrate:
	$(CC) calibrate.c -o calibrate

clean:
	rm -rf *o sbench calibrate
