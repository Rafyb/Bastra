
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



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
        /* traitement de l ’ erreur */
    }
  
    if ( bind ( socket_serveur , ( struct sockaddr *)& saddr , sizeof ( saddr )) == -1) {
        perror ( " bind socker_serveur " );
        /* traitement de l ’ erreur */
    }
    if ( listen ( socket_serveur , 10) == -1)   {
        perror ( " listen socket_serveur " );
        /* traitement d ’ erreur */
    }

    int socket_client ;
    int pid = 1;
    while(pid) {
        socket_client = accept ( socket_serveur , NULL , NULL );
        if ( socket_client == -1){
            perror ( " accept " );
            /* traitement d ’ erreur */
        }
        printf("Client connected");
        pid = fork();
        if(pid) {
            close(socket_client);
        }
    }    
    
    /* On peut maintenant dialoguer avec le client */
    sleep(1);

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
    write ( socket_client , message_bienvenue1 , strlen ( message_bienvenue1 ) );
    write ( socket_client , message_bienvenue2 , strlen ( message_bienvenue2 ));
    write ( socket_client , message_bienvenue3 , strlen ( message_bienvenue3 ));
    write ( socket_client , message_bienvenue4 , strlen ( message_bienvenue4 ));
    write ( socket_client , message_bienvenue5 , strlen ( message_bienvenue5 ));
    write ( socket_client , message_bienvenue6 , strlen ( message_bienvenue6 ));
    write ( socket_client , message_bienvenue7 , strlen ( message_bienvenue7 ));
    write ( socket_client , message_bienvenue8 , strlen ( message_bienvenue8 ));
    write ( socket_client , message_bienvenue9 , strlen ( message_bienvenue9 ));
    write ( socket_client , message_bienvenue10 , strlen ( message_bienvenue10 ));
    
    /* Lecture de l'entrée et renvoie le message */
    const int TAILLE_MAX = 255;
    while(1){
        char buffer_tmp[TAILLE_MAX];
        int size = read(socket_client, buffer_tmp,TAILLE_MAX);
        if(size>TAILLE_MAX){
            write(socket_client, "Chaine trop grande\n",19);
        } else {
        write(socket_client, message, size);
        char * message = malloc(size);
        strcpy(message, buffer_tmp);
        } 
    }

    return socket_serveur;
}

