# include <stdio.h>
# include <string.h>
#include "socket.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "http_parse.h"

const int TAILLE_MAX = 255;
char * fgets_or_exit ( char * buffer , int size , FILE * stream ){
    char * re = fgets(buffer,size,stream);
    if( re =='\0'){
        exit(1);
    }
    return re;
}


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
    /* Gestion fermeture d'un processus */
    struct sigaction sa ;
    sa . sa_handler = traitement_signal ;
    sigemptyset (& sa . sa_mask );
    sa . sa_flags = SA_RESTART ;
    if ( sigaction ( SIGCHLD , & sa , NULL ) == -1)
    {
    perror ( " sigaction ( SIGCHLD ) " );
    }

   
}

void skip_headers(FILE *f) {
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    char* arg = "%s";
    fgets(buffer,TAILLE_MAX,f);
    while(strcmp(buffer,"\r\n")!=0) {
        printf(arg,buffer);
        fgets(buffer,TAILLE_MAX,f);
    }
}

int main(void)
{
    /* message bienvenue */
    const char * message_bienvenue1 = " _______                         __                          \n" ;
    const char * message_bienvenue2 = "/       \\                       /  |                         \n";
    const char * message_bienvenue3 = "███████  |  ______    _______  _██ |_     ______    ______   \n";
    const char * message_bienvenue4 = "██ |__██ | /      \\  /       |/ ██   |   /      \\  /      \\  \n" ;
    const char * message_bienvenue5 = "██    ██<  ██████  |/███████/ ██████/   /██████  | ██████  | \n" ;
    const char * message_bienvenue6 = "███████  | /    ██ |██      \\   ██ | __ ██ |  ██/  /    ██ | \n" ;
    const char * message_bienvenue7 = "██ |__██ |/███████ | ██████  |  ██ |/  |██ |      /███████ | \n" ;
    const char * message_bienvenue8 = "██    ██/ ██    ██ |/     ██/   ██  ██/ ██ |      ██    ██ | \n";
    const char * message_bienvenue9 = "███████/   ███████/ ███████/     ████/  ██_/       ███████/   \n" ;
    const char * message_bienvenue10 = " \n" ;
    const char * message_bienvenue11 = " \n";
    const char * message_bienvenue12 = "\n";
    int size = 0;
    size += strlen(message_bienvenue1) + strlen(message_bienvenue1) +  strlen(message_bienvenue2) +  strlen(message_bienvenue3) + 
    strlen(message_bienvenue4) +  strlen(message_bienvenue5) +  strlen(message_bienvenue6) +  strlen(message_bienvenue7) + 
    strlen(message_bienvenue8) + strlen(message_bienvenue9) + strlen(message_bienvenue10) + strlen(message_bienvenue11) + 
    strlen(message_bienvenue12); 
    
    /* initialisation */
    initialiser_signaux();
    int socket_serveur = creer_serveur(8080);

    int socket_client ;
    int pid = 1;

    while(pid) {
        socket_client = accept ( socket_serveur , NULL , NULL );
        
        if ( socket_client == -1){
            perror ( " accept " );
            /* traitement d ’ erreur */
        }
        pid = fork();
        if(pid) {
            close(socket_client);
        }
    }    
 
    
    FILE *f = fdopen(socket_client,"w+"); 
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    char* arg = "%s";
    fgets_or_exit(buffer,TAILLE_MAX,f);

    /* lire lien entete 
    char* lien = strstr(buffer,"/");
    if (lien==NULL){
        char * error = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
        fprintf(f,arg,error);
        exit(1);
    }
    printf("%s\n",lien);
    */


    /* verification methode appelee */
    http_request request; 
    parse_http_request( buffer , &request );
    
    if(strcmp(buffer,"GET /inexistant HTTP/1.1\r\n")==0){ // ERREUR 404
        char* error = "HTTP/1.1 404 Not found\r\nConnection: close\r\nContent-Length: 18\r\n\r\n404 Not found\r\n";
        fprintf(f,arg,error);
        exit(404);
    }

    if(strcmp(buffer,"GET / HTTP/1.1\r\n")!=0){ // ERREUR 400
        char * error = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
        fprintf(f,arg,error);
        exit(1);
    }
    skip_headers(f);

    /* requete correcte */
    arg = "%s %d%s";
    fprintf(f,arg,"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length:",size,"\r\n\r\n");
    fprintf(f,"%s%s%s%s%s%s%s%s%s%s%s%s",message_bienvenue1,message_bienvenue2,message_bienvenue3,message_bienvenue4,
    message_bienvenue5,message_bienvenue6,message_bienvenue7,message_bienvenue8,
    message_bienvenue9,message_bienvenue10,message_bienvenue11,message_bienvenue12);
   

    return 0;
}
