
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>



void traitement_signal (int sig) {
    //printf ( " Signal %d reçu \n " , sig );
    //waitpid(pid,NULL,WNOHANG);
    if(SIGCHLD == sig){
        waitpid(-1,NULL,WNOHANG);
    }
    //exit(0);
}

void initialiser_signaux ( void ) {
    if ( signal ( SIGPIPE , SIG_IGN ) == SIG_ERR )  {
        perror ( " signal " );
    }
    struct sigaction sa ;
    sa . sa_handler = traitement_signal ;
    sigemptyset (& sa . sa_mask );
    sa . sa_flags = SA_RESTART ;
    if ( sigaction ( SIGCHLD , & sa , NULL ) == -1)
    {
    perror ( " sigaction ( SIGCHLD ) " );
    }

   
}

int creer_serveur(int port) {
    int socket_serveur;
    socket_serveur = socket ( AF_INET , SOCK_STREAM , 0);
    if ( socket_serveur == -1)  {
        /* traitement de l ’ erreur */
        perror( " socket_serveur " );
    }
    /* Utilisation de la socket serveur */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET; /* Socket ipv4 */
    saddr.sin_port = htons(port); /* Port d ’écoute */
    saddr.sin_addr.s_addr = INADDR_ANY ; /* écoute sur toutes les interfaces */
    int optval = 1;
    if ( setsockopt ( socket_serveur , SOL_SOCKET , SO_REUSEADDR , & optval , sizeof ( int )) == -1) {
        perror ( " Can not set SO_REUSEADDR option " );
    }
  
    if ( bind ( socket_serveur , ( struct sockaddr *)& saddr , sizeof ( saddr )) == -1) {
        perror ( " bind socker_serveur " );
        /* traitement de l ’ erreur */
    }
    if ( listen ( socket_serveur , 10) == -1)   {
        perror ( " listen socket_serveur " );
        /* traitement d ’ erreur */
    }
    initialiser_signaux();
    int socket_client ;
    int pid = 1;
    while(pid) {
    socket_client = accept ( socket_serveur , NULL , NULL );
    if ( socket_client == -1)
    {
    perror ( " accept " );
    /* traitement d ’ erreur */
    }
    pid = fork();
    if(pid) {
        close(socket_client);
    }
    }    
    /* On peut maintenant dialoguer avec le client */
    

    const char * message_bienvenue1 = "  __________________________________________\n " ;
    const char * message_bienvenue2 = "|Bonjour , bienvenue sur le serveur Bastra |\n " ;
    const char * message_bienvenue3 = "|__________________________________________|\n ";
    const char * message_bienvenue4 = "|Bonjour , bienvenue sur le serveur Bastra |\n " ;
    const char * message_bienvenue5 = "|Bonjour , bienvenue sur le serveur Bastra |\n " ;
    const char * message_bienvenue6 = "|Bonjour , bienvenue sur le serveur Bastra |\n " ;
    const char * message_bienvenue7 = "|Bonjour , bienvenue sur le serveur Bastra |\n " ;
    const char * message_bienvenue8 = "|Bonjour , bienvenue sur le serveur Bastra |\n " ;
    const char * message_bienvenue9 = "|Bonjour , bienvenue sur le serveur Bastra |\n " ;
    const char * message_bienvenue10 = "|__________________________________________|\n " ;
    write ( socket_client , message_bienvenue1 , strlen ( message_bienvenue1 ));
    write ( socket_client , message_bienvenue2 , strlen ( message_bienvenue2 ));
    write ( socket_client , message_bienvenue3 , strlen ( message_bienvenue3 ));
    write ( socket_client , message_bienvenue4 , strlen ( message_bienvenue4 ));
    write ( socket_client , message_bienvenue5 , strlen ( message_bienvenue5 ));
    write ( socket_client , message_bienvenue6 , strlen ( message_bienvenue6 ));
    write ( socket_client , message_bienvenue7 , strlen ( message_bienvenue7 ));
    write ( socket_client , message_bienvenue8 , strlen ( message_bienvenue8 ));
    write ( socket_client , message_bienvenue9 , strlen ( message_bienvenue9 ));
    write ( socket_client , message_bienvenue10 , strlen ( message_bienvenue10 ));
    sleep(1);
    
    

    return socket_serveur;
}

