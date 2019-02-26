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

void send_status( FILE * client , int code , const char * reason_phrase ){
    fprintf(client,"%d%s%s",code,reason_phrase,"\r\n");
}

void send_response ( FILE * client , int code , const char * reason_phrase , const char * message_body ){
    send_status( client , code , reason_phrase );
    int size = strlen(message_body); 
    char* arg = "%s%d%s%s";
    fprintf(client,arg,"Connection: close\r\nContent-Length:",size,"\r\n\r\n",message_body);
}

int main(void)
{
    /* message bienvenue */
    char * message_bienvenue = " _______                         __                          \n/       \\                       /  |                         \n███████  |  ______    _______  _██ |_     ______    ______   \n██ |__██ | /      \\  /       |/ ██   |   /      \\  /      \\  \n██    ██<  ██████  |/███████/ ██████/   /██████  | ██████  | \n███████  | /    ██ |██      \\   ██ | __ ██ |  ██/  /    ██ | \n██ |__██ |/███████ | ██████  |  ██ |/  |██ |      /███████ | \n██    ██/ ██    ██ |/     ██/   ██  ██/ ██ |      ██    ██ | \n███████/   ███████/ ███████/     ████/  ██_/       ███████/  \n\n\n\n";
    /*size += strlen(message_bienvenue1) + strlen(message_bienvenue1) +  strlen(message_bienvenue2) +  strlen(message_bienvenue3) + 
    strlen(message_bienvenue4) +  strlen(message_bienvenue5) +  strlen(message_bienvenue6) +  strlen(message_bienvenue7) + 
    strlen(message_bienvenue8) + strlen(message_bienvenue9) + strlen(message_bienvenue10) + strlen(message_bienvenue11) + 
    strlen(message_bienvenue12); */

    
    
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
    fgets_or_exit(buffer,TAILLE_MAX,f);


    /* verification methode appelee */
    http_request request; 

    

    if( !parse_http_request( buffer , &request ))
        send_response( f, 400 , "Bad Request" , "Bad request\r\n" );
    else if( request.method == HTTP_UNSUPPORTED )
        send_response( f , 405 , "Method Not Allowed" , "Method Not Allowed\r\n" );
    else if( strcmp( request.target , "/" ) == 0)
        send_response( f , 200 , " OK " , message_bienvenue );
    else
        send_response( f , 404 , "Not Found" , "Not Found \r\n");

    /*
    if(strcmp(buffer,"GET /inexistant HTTP/1.1\r\n")==0){ // ERREUR 404
        send_response( f , 404 , "Not found", "Connection: close\r\nContent-Length: 18\r\n\r\n404 Not found\r\n" );
        char* error = "HTTP/1.1 404 Not found\r\nConnection: close\r\nContent-Length: 18\r\n\r\n404 Not found\r\n";
        fprintf(f,arg,error);
        exit(404);
    }

    if(strcmp(buffer,"GET / HTTP/1.1\r\n")!=0){ // ERREUR 400
        char * error = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
        fprintf(f,arg,error);
        exit(1);
    }
    */
    skip_headers(f);

    return 0;
}
