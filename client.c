/****************************************************************\
* (client.c) Client for reading input messages from keybd. *
\****************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

main() {
        int sservice, end;
        char buf[128];
        struct protoent *ppe;
        struct sockaddr_in sout;
        ppe = getprotobyname("tcp");/*Get the tcp-entity data*/
/*Open the socket*/
        sservice=socket(PF_INET,SOCK_STREAM,ppe->p_proto);
        sout.sin_family = AF_INET; /*ARPANET */
        sout.sin_addr.s_addr = getservbyport(4000,"tcp"); /* Which
                                                             server?*/
        sout.sin_port = 4000; /*Output port*/
/*Connect to the server*/
        connect(sservice,(struct sockaddr *)&sout, sizeof(sout));
        end = FALSE;
        while(!end) /*Client Service Loop*/
        { /*Ask for the service*/
                printf("\nCLIENT> Send a message...: ");
                fflush(stdout);
                scanf("%s",buf);
                write(sservice,buf,strlen(buf));
                printf("\nCLIENT>Client sent: %s", buf);
                fflush(stdout);
                if (!strcmp(buf,"end"))
                {
                        printf("\nCLIENT>Bye..."); fflush(stdout);
                        end=TRUE;
                }
        }
        close(sservice);
} /*main()*/
