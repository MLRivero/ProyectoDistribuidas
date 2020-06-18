/****************************************************************\
*(server.c) The Server shows received messages in the screen. *
\****************************************************************/
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0

void handlerChld(int);
void handlerTimer(int);

int nClientes = 0;
void main() {
  int sservice, sclient, l = 0, nbytes_read, end;
  char buf[128];
  struct protoent *ppe;
  struct sockaddr_in sin, clientfsin;
  ppe = getprotobyname("tcp");
  struct itimerval t;
  t.it_interval.tv_sec = 2; // sets 2s timer and 2s interval
  t.it_interval.tv_usec = 0;
  t.it_value.tv_sec = 2;
  t.it_value.tv_usec = 0;

  signal(SIGCHLD, handlerChld);
  signal(SIGALRM, handlerTimer);
  /*Open the socket*/
  sservice = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
  if (sservice == -1)
    perror("No se pudo abrir el socket");
  /*pse = getservbyname("messages", "tcp"); <---If service is reg.*/
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = 4000;

  sclient =
      bind(sservice, (struct sockaddr *)&sin, sizeof(sin)); /*Register serv*/
  if (sclient == -1)
    perror("No se pudo hacer el bind");
  listen(sservice, 5); /*Up to 5 pending connections*/

  /*If concurrent, spawn children server here by using fork()*/
  end = FALSE;
  setitimer(ITIMER_REAL, &t, NULL);
  while (!end) /*Give the service*/
  {

    sclient = accept(sservice, (struct sockaddr *)&clientfsin,
                     &l); /*Accept a client*/
    if (sclient == -1)
      perror("No se pudo aceptar cliente");
    nClientes++;

    if (fork() == 0) {
    printf("Conectado nuevo cliente %d con ID %d\n", nClientes, getpid());
      while (!end) {
        nbytes_read = read(sclient, buf, sizeof(buf));
        if (nbytes_read >= 0)
          buf[nbytes_read] = '\0';
        else {
          strcpy(buf, "\nSERVER>ERROR IN SERVER!");
          end = TRUE;
        }
        printf("\nSERVER>Server received: %s de cliente %d\n", buf, getpid());
        fflush(stdout);
        if (!strcmp(buf, "end"))
          end = TRUE;
        /*Never forget to close a socket!*/
      }
      exit(0);
    }
  }
  close(sclient);

  printf("\nSERVER>Bye...");
  fflush(stdout);

  close(sservice);
} /*main()*/

void handlerChld(int s) {
  printf("El cliente con PID %d sale\n", getpid());
  nClientes--;
}

void handlerTimer(int s) {
  if (nClientes > 0)
    printf("Número de clientes conectados: %d\n", nClientes);
}
