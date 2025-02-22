# Library Service

A simple channels mechanism to make communication between threads.

## Simple

Just make a clone the project, compile the .c, and run the executable.

## Usage

There's a couple of threads that will communicate with each other in .c once you will run the program, it is a use of the channel service.

The channel is declared in the main as pointer to a channel, and this is not optional since you must intialize it with the init_channel() function and this function returns a pointer to an initialized channel. NOTE: you must initialize the channel, after declaring it as a pointer.

Now you have created a channel, it's time to use it, I mean to send/receive data through it. There's the two functions for this perpose, send(), recieve().
The "void send(Channel *ch, int data)" will take two parameters, a pointer to the channel you want a thread to send data to, the data that you want a thread to send through the channel.
The "void recieve(Channel *ch)" will take one parameter, a pointer to the channel you want a thread to take the data from.

## What is the Channel?

The channel itself is a struct that will holds a buffer that will holds the data, indexes to deal with the buffer, mutex, and condition varibles.

## What is the buffer?

It is the (Ring buffer) data structure, it acts as a (Cirqular queue).
You can replace this data structure with any one that you prefer, but, take care about the condition variables, here in this project the condition varibles are declared and used especially for the (Ring buffer), so keep that in mind.
Also you need to take care about the mutex if you used a nother data structure.
