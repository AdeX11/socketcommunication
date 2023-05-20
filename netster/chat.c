 #include <unistd.h>
 #include <netinet/in.h>
 #include <netinet/tcp.h>
 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stddef.h>
 #include <arpa/inet.h>
 #include <netdb.h>

 
/*
 *  Here is the starting point for your netster part.1 definitions. Add the 
 *  appropriate comment header as defined in the code formatting guidelines
 */

/* listens for incoming connections and responds to messages from clents */
void chat_server(char* iface, long port, int use_udp) {
  char a[32] ;
  sprintf(a,"%ld", port);
  struct sockaddr address;
  int serverSock=0, b ;
  struct addrinfo hints;
  struct addrinfo* addr;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if(use_udp)
    hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  if(getaddrinfo(iface, a, &hints, &addr) != 0)
    exit(EXIT_FAILURE);
  while (addr != NULL)
  {
    serverSock = socket(addr->ai_family,addr->ai_socktype,addr->ai_protocol);
    if(serverSock  == -1)
    {
      continue;
    }
    if(bind(serverSock,addr->ai_addr, addr->ai_addrlen) == 0)
      break;
    close(serverSock);
    addr =addr->ai_next;
  }
  if(addr == NULL)
    exit(EXIT_FAILURE);
  freeaddrinfo(addr);
  if(!use_udp)
  { 
    if(listen(serverSock,1024) == -1)
    {
      printf("listening failed\n");
    }
    int num = 0;
    while (1)
    {
      char msg[256];
      char msgrcv[256];
      socklen_t len = sizeof(address);
      int conSocket =accept(serverSock,&address, &len);
      char host[NI_MAXHOST], service[NI_MAXSERV];
      b = getnameinfo(&address,
                     len, host, NI_MAXHOST,
                     service, NI_MAXSERV, NI_NUMERICSERV);
      //b = getpeername(serverSocket, &peer_addr, sizeof(peer_addr));
      if(b != 0)
        exit(EXIT_FAILURE);
      printf("connection %d from (%s, %s)\n", num++, host, service);
      memset(msgrcv, '\0',sizeof(msgrcv));
      while(recv(conSocket,msgrcv,256, 0) != -1)
      {
        printf("got message from ('%s', %s)\n",host,service);
        if(msgrcv[strlen(msgrcv)-1] != '\n')
        {
          printf("received sus\n");
        }
        if(strcmp(msgrcv, "hello\n") == 0)
        {
          memset(msg, '\0',sizeof(msg));
          strcpy(msg,"world\n");
          if(send(conSocket, msg, sizeof(msg),0) == 0)
          {
            printf("send failed\n");
            return ;
          }
        }
        else if(strcmp(msgrcv, "goodbye\n") == 0)
        {
          memset(msg, '\0',sizeof(msg));
          strcpy(msg,"farewell\n");
          if(send(conSocket, msg, sizeof(msg),0) == 0)
          {
            printf("send failed\n");
            return ;
          }
          break;
        }
        else if(strcmp(msgrcv, "exit\n") == 0)
        {
          memset(msg, '\0',sizeof(msg));
          strcpy(msg,"ok\n");
          if(send(conSocket, msg, sizeof(msg),0) == 0)
          {
            printf("send failed\n");
            return;
          }
          close(conSocket);
          return;
        }
        else
        {
          if(send(conSocket,msgrcv, sizeof(msgrcv),0) == 0)
          {
            printf("send failed\n");
            return;
          }
        }
        memset(msgrcv, '\0',sizeof(msgrcv));
      }
    
     }
  }
  else
  {
    char msg[256];
    char msgrcv[256];
    memset(msgrcv, '\0',sizeof(msgrcv));
    while(1)
    {
      int size;
      socklen_t len =sizeof(address); 
      if((size =recvfrom(serverSock,msgrcv,256,0,&address, &len)) ==-1)
      {
        printf("receive failed/n");
        continue;
      }
      char host[NI_MAXHOST],service[NI_MAXSERV];
      b = getnameinfo((struct sockaddr *) &address,
                               len, host, NI_MAXHOST,
                               service, NI_MAXSERV, NI_NUMERICSERV);
      if (b != 0)
        exit(EXIT_FAILURE);
      printf("got message from ('%s', %s)\n",host,service);
      if(strcmp(msgrcv, "hello\n") == 0)
      {
        memset(msg, '\0',sizeof(msg));
        strcpy(msg,"world\n");
        if(sendto(serverSock, msg, sizeof(msg),0,&address,len) == 0)
        {
          printf("send failed\n");
          return ;
        }
      }
      else if(strcmp(msgrcv, "goodbye\n") == 0)
      {
        memset(msg, '\0',sizeof(msg));
        strcpy(msg,"farewell\n");
        if(sendto(serverSock, msg, sizeof(msg),0,&address,len)==0)
        {
          printf("send failed\n");
          return ;
        }
        break;
      }
      else if(strcmp(msgrcv, "exit\n") == 0)
      {
        memset(msg, '\0',sizeof(msg));
        strcpy(msg,"ok\n");

        if(sendto(serverSock, msg, sizeof(msg),0,&address,len) == 0)
        {
          printf("send failed\n");
          return;
        }
        close(serverSock);
        return;
      }
      else
      {
        if(sendto(serverSock, msgrcv, sizeof(msgrcv),0,&address,len) != size)
        {
          printf("send failed\n");
          return;
        }
      }
      memset(msgrcv, '\0',sizeof(msgrcv));

    }

  }


}


void chat_client(char* host, long port, int use_udp) {
  //printf("Hello, I am a client\n");
  char a[32] ;
  sprintf(a,"%ld", port);
  int clientSock =0;
 // struct sockaddr address;
  //struct sockaddr add;
  struct addrinfo hints;
  struct addrinfo* addr;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if(use_udp)
    hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(host, a, &hints, &addr); 
  while (addr != NULL)
  { 
    clientSock =  socket(addr->ai_family,addr->ai_socktype,addr->ai_protocol);
    if(clientSock == -1)
      continue;
    if(connect(clientSock,addr->ai_addr,addr->ai_addrlen) == 0)
    {
      break;
    }
    close(clientSock);
    addr =addr->ai_next;
  }
  freeaddrinfo(addr);
  char msg[256];
  char msgrcv[256];
  while (1)
  {
    if(!use_udp)
    {
      memset(msg,0, sizeof(msg));
      memset(msgrcv,0,sizeof(msgrcv));
      if(fgets(msg, 256, stdin) == NULL)
      {
        printf("user input was not received\n");
      }
      if(send(clientSock, msg, 256,0) == 0)
      {
        printf("send failed\n");
      }
      if(recv(clientSock,msgrcv,256, 0) == -1)
      {
        printf("receive failed\n");
      }
      printf("%s",msgrcv);
      if((strcmp(msgrcv,"ok\n") == 0 && strcmp(msg,"ok\n") != 0) || (strcmp(msgrcv, "farewell\n")==0 
      && strcmp(msg, "farewell\n") != 0))
      {
        break;
      }  
    }
    else
    {
      //socklen_t len = sizeof(address);
      memset(msg,0, sizeof(msg));
      memset(msgrcv,0,sizeof(msgrcv));
      if(fgets(msg, 256, stdin) == NULL)
      {
        printf("user input was not received\n");
      }
      if(write(clientSock, msg, 256)  != 256)
      {
        printf("send failed\n");
      }
      if(read(clientSock,msgrcv,256) == -1)
      {
        printf("receive failed\n");
      }
      printf("%s",msgrcv);
      if(msgrcv[strlen(msgrcv)-1] != '\n')
      {
        break;
      }
      if((strcmp(msgrcv,"ok\n") == 0 && strcmp(msg,"ok\n") != 0) || (strcmp(msgrcv, "farewell\n")==0
      && strcmp(msg, "farewell\n") != 0))
      {
        break;
      }
    }

  }
  exit(EXIT_SUCCESS);
  return;
 
}
