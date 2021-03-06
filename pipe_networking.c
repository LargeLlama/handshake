#include "pipe_networking.h"

//make a global variable for the name of the pipe to make it easier to call upon it
char * SERVER_PIPE = "mario";
char * CLIENT_PIPE = "luigi";


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) 
{
	//make the pipe and remove any existing ones
	printf("[server] Making the upstream pipe :3\n");
	remove(SERVER_PIPE);

	int pipe = mkfifo(SERVER_PIPE, 0666);
	//error handling
 	if (pipe < 0)
	{
		printf("[server] UH OH couldn't make the pipe gotta blast\n");
		exit(0);
	}

	printf("[server] downstream pipe made :3\n");

	//opening the pipe and removing it after connection established
	to_client = open(SERVER_PIPE, O_RDONLY);
	printf("[server] downstream pipe connected with client uwu, removing it :3\n");
	remove(SERVER_PIPE);

	//receiving the data from the pipe
	char data[10];
	read(to_client, data, 10); 
	printf("[server] received data from client :3\n");
	printf("[server] gonna connect to the client brb\n");

	//opens the upstream pipe based off the data provided by the client
	int upstream = open(data, O_WRONLY);
	char response[HANDSHAKE_BUFFER_SIZE];
	read(upstream, response, HANDSHAKE_BUFFER_SIZE);

	printf("[server] client has received our response and also responded :3\n");
	printf("[server] 3-WAY HANDSHAKE COMPLETED! GOOD JOB LADS, PULLING OUT NOW\n");

	return upstream;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server)
{
	printf("[client] making super personal private pipe :3\n");

	int private = mkfifo(CLIENT_PIPE, 0666);

	if(private < 0)
	{
		printf("[client] oopsie woopsie, couldn't make the private pipe :(\n");
		exit(0);
	}

	printf("[client] connecting to the server's pipe O.o\n");
	int down = open(SERVER_PIPE, O_WRONLY);
	printf("[client] eureka! i done did it! gonna pass them my private name brb\n");

	write(down, CLIENT_PIPE, strlen(CLIENT_PIPE));
	printf("[client] sent it :3\n");
	remove(CLIENT_PIPE);

	to_server = open(CLIENT_PIPE, O_RDONLY);
	char msg[HANDSHAKE_BUFFER_SIZE];

	read(to_server, msg, HANDSHAKE_BUFFER_SIZE);
	printf("[client] connection established with my homie, gonna remove unecessary pipes now :3\n");
	remove(private);
	
	printf("[client] letting my broski know i got the msg uwu\n");
	write(to_server, ACK, strlen(ACK) + 1);
	
	printf("[client] done!\n");

	return to_server;
}
