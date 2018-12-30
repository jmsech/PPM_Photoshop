#Makefile
#Justin Sech and Ire Shobayo
#602.220 Fall 2018, Midterm Project

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

# Links together files needed to create executable
project: project.o image_manip.o ppm_io.o
	$(CC) -o project project.o image_manip.o ppm_io.o -lm

# Compiles project.c to create project.o
project.o: project.c image_manip.h ppm_io.h
	$(CC) $(CFLAGS) -c project.c

# Compiles image_manip.c to create image_manip.o
image_manip.o: image_manip.c image_manip.h
	$(CC) $(CFLAGS) -c image_manip.c
#compiles ppm_io.c to create ppm_io.o
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

# Removes all object files and the executable named project,
# so we can start fresh
clean:
	rm -f *.o project
