
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int freading(int socket_client) {
    const int TAILLE_MAX = 255;
    FILE *f = fdopen(socket_client,"w+");
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    while(fgets(buffer,TAILLE_MAX,f)!=NULL) {
        char* arg = "<BastRa> %s";
        fprintf(f,arg,buffer);
    }
    return 1;
}

int reading(int socket_client){
    const int TAILLE_MAX = 255;
    char buffer_tmp[TAILLE_MAX];
    int size;
    
    while((size = read(socket_client, buffer_tmp,TAILLE_MAX))){
        if(size>TAILLE_MAX){
            write(socket_client, "Chaine trop grande\n",19);
        } else {
                char * message = malloc(size);
            strcpy(message, buffer_tmp);
            write(socket_client, message, size);
        } 
    }
    return 1;
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
        //printf("Client connected");
        pid = fork();
        if(pid) {
            close(socket_client);
        }
    }    
    
    /* On peut maintenant dialoguer avec le client */
    sleep(1);

    const int TAILLE_MAX = 255;
    FILE *f = fdopen(socket_client,"w+");
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
        char* arg = "%s";
    
    fgets(buffer,TAILLE_MAX,f);
    if(strcmp(buffer,"GET / HTTP/1.1\r\n")!=0){
        char * error = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
        fprintf(f,arg,error);
    }
    printf(arg,buffer);
    

    const char * message_bienvenue1 = " _______                         __                          \n" ;
    const char * message_bienvenue2 = "/       \\                       /  |                         \n";
    const char * message_bienvenue3 = "███████  |  ______    _______  _██ |_     ______    ______   \n";
    const char * message_bienvenue4 = "██ |__██ | /      \\  /       |/ ██   |   /      \\  /      \\  \n" ;
    const char * message_bienvenue5 = "██    ██<  ██████  |/███████/ ██████/   /██████  | ██████  | \n" ;
    const char * message_bienvenue6 = "███████  | /    ██ |██      \\   ██ | __ ██ |  ██/  /    ██ | \n" ;
    const char * message_bienvenue7 = "██ |__██ |/███████ | ██████  |  ██ |/  |██ |      /███████ | \n" ;
    const char * message_bienvenue8 = "██    ██/ ██    ██ |/     ██/   ██  ██/ ██ |      ██    ██ | \n";
    const char * message_bienvenue9 = "███████/   ███████/ ███████/     ████/  ██_/        ███████/  \n" ;
    const char * message_bienvenue10 = " \n" ;
    const char * message_bienvenue11 = " \n";
    const char * message_bienvenue12 = "\n";
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
    write ( socket_client , message_bienvenue11 , strlen ( message_bienvenue11 ));
    write ( socket_client , message_bienvenue12 , strlen ( message_bienvenue12 ));
    
    /* Lecture de l'entrée et renvoie le message */
    //freading(socket_client);
 
    
    return socket_serveur;
}

