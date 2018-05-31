ioreceive: src/ioreceive.c
	gcc -o bin/ioreceive src/ioreceive.c -I./src

iosend: src/iosend.c
	gcc -o bin/iosend src/iosend.c -I./src

display: src/display.c
	gcc -o bin/spawn src/display.c -I./src

spawn: src/spawn.c
	gcc -o bin/spawn src/spawn.c -I./src
