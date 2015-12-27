
all : missile
	
missile : missileLauncher.c discoverUSB.c arrowControl.c
	gcc -o missile -g3 -std=c99 missileLauncher.c arrowControl.c discoverUSB.c -lusb
