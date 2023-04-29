#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define BACKLOG 10     /* numar de conexiuni  concurente*/

void print_message(char* message);

int main(int argc, char* argv[]) {
  if(argc != 4) {
    printf("Usage: %s <ip_address> <port> <multiple>\n", argv[0]);
    return 1;
  }

  char *ip_address = argv[1];
  printf("IP address: %s\n", ip_address);
  int port = atoi(argv[2]);
  int multiple = atoi(argv[3]);
  char message[256];
  strcpy(message, argv[1]);
  strcat(message, ": ACK\n");

  int network_socket = socket(AF_INET, SOCK_DGRAM, 0);

  //Specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = INADDR_ANY;

  //Bind the socket to the address and port
  bind(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

  //Accept the connection
  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);

  printf("Waiting for message...\n");
  
  char client_message[256];
  do {
    recvfrom(network_socket, client_message, 256, 0, (struct sockaddr *) &client_address, &client_address_len);
    if(atoi(client_message) % multiple == 0) {
      printf("%d\n", atoi(client_message));
      sendto(network_socket, message, strlen(message), 0, (struct sockaddr *) &client_address, client_address_len);
    }
    memset(client_message, 0, 256);
  } while(1);

  printf("Message received!\n");
  return 0;
}

void print_message(char* message) {
  char msg_aux[256];

  int i;
  for(i = 0; i < strlen(message); i++) {
    msg_aux[i] = message[i];

    if(message[i] == '\n')
      break;
  }
  msg_aux[i] = '\0';

  printf("Server says: %s\n", msg_aux);
}
