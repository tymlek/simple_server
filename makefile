CPP = gcc
source = simple_server.c
output = simple_server
objects = $(output).o
CFLAGS =  -c -std=c11

$(output): $(objects)
	$(CPP) $(objects) -o $(output) 

$(objects): $(source) hacking.h
	$(CPP) $(CFLAGS) $(source)

clean:
	rm $(objects)
	rm $(output)
