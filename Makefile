CC=gcc

all: sbench calibrate

sbench: sbench.c
	$(CC) sbench.c -o sbench

calibrate: calibrate.c
	$(CC) calibrate.c -o calibrate

clean:
	rm -rf *o sbench calibrate
