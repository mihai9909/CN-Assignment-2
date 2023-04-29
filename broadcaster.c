#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define MYPORT 9434    /* numar port va fi completat corespunzator */
#define BACKLOG 10     /* numar de conexiuni  concurente*/
#define CLIENT_1_PORT 9435
#define CLIENT_2_PORT 9436

struct socket_and_address {
  int socket;
  struct sockaddr_in address;
};

void send_to_client(int network_socket, int port, char* ip_address, int i);
void *received_messages(void *vargp);

int main(int argc, char* argv[]) {

  int network_socket = socket(AF_INET, SOCK_DGRAM, 0);

  //Specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(MYPORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  // Set the socket to non-blocking mode
  int flags = fcntl(network_socket, F_GETFL, 0);
  fcntl(network_socket, F_SETFL, flags | O_NONBLOCK);

  //bind the socket
  if(bind(network_socket, (struct sockaddr *) &server_address, sizeof(server_address))){
    printf("Error binding socket: %s\n", strerror(errno));
    return 1;
  }

  struct socket_and_address file_descr_and_address;
  file_descr_and_address.socket = network_socket;
  file_descr_and_address.address = server_address;

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, received_messages, &file_descr_and_address);

  char buffer[256];
  do {
    for(int i = 1; i <= 100; i++) {
      // generate random number
      int random_number = rand() % 2;
      if(random_number == 0) {
        send_to_client(network_socket, CLIENT_1_PORT, "127.0.0.2", i);
      } else{
        send_to_client(network_socket, CLIENT_2_PORT, "127.0.0.3", i);
      }
    }  
    scanf("%s", buffer);
  } while(strcmp(buffer, "exit") != 0);

  return 0;
}

void send_to_client(int network_socket, int port, char* ip_address, int i) {
  //Accept the connection
  struct sockaddr_in client_address;
  
  //Set client ip and port to client_address
  client_address.sin_family = AF_INET;
  client_address.sin_port = htons(port);
  client_address.sin_addr.s_addr = inet_addr(ip_address);
  socklen_t client_address_len = sizeof(client_address);

  //Send the message
  char server_message[256];
  sprintf(server_message, "%d", i);
  sendto(network_socket, server_message, sizeof(server_message), 0, (struct sockaddr *) &client_address, client_address_len);
}

void *received_messages(void *vargp) {
  char buffer[256];
  struct socket_and_address socket_and_address = *((struct socket_and_address *) vargp);
  int network_socket = socket_and_address.socket;
  struct sockaddr_in client_address = socket_and_address.address;
  socklen_t client_address_len = sizeof(client_address);

  while(1) {
    memset(buffer, 0, sizeof(buffer));
    recvfrom(network_socket, buffer, 256, 0, (struct sockaddr *) &client_address, &client_address_len);
    printf("%s", buffer);
  }
  return NULL;
}
