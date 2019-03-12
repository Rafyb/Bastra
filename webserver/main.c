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
#include <dirent.h>
#include <sys/stat.h>


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
    char* buf = malloc(sizeof(char)*TAILLE_MAX);
    char* arg = "%s";
    fgets_or_exit(buf,TAILLE_MAX,f);
    while(strcmp(buf,"\r\n")!=0) {
        printf(arg,buf);
        fgets_or_exit(buf,TAILLE_MAX,f);
    }
}

void send_status( FILE * client , int code , const char * reason_phrase ){
    fprintf(client,"HTTP/1.1 %d %s\r\n",code,reason_phrase);
}

void send_response ( FILE * client , int code , const char * reason_phrase , const char * message_body ){
    send_status( client , code , reason_phrase );
    fprintf(client, "Connection: close\r\n");

	int size = strlen(message_body);
    printf("Content Length: %d\r\n",size);
    
    fprintf(client, "Content Length: %d\r\n",size);
    fprintf(client, "\r\n%s\r\n",message_body);
}

char *rewrite_target(char *target) {
    
    char* ok = strchr(target,'?');
    char* res;
    if(ok !=NULL) {
       // printf("%s\n",ok);
       int size = strlen(target) - strlen(ok);
       res = malloc(size);
       strncpy(res,target,size);
    } else {
        
        return target;
    }
    return res;
}
FILE * check_and_open ( const char * target , const char * document_root ) {
    struct stat buf;
    //DIR *rep = opendir(argv[1]);
    stat(document_root, &buf);
    if(!S_ISDIR(buf.st_mode)) {
        perror("erreur répertoire n'existe pas");
        exit(1);
    }
    char* link = malloc(sizeof(char)*100);
    //printf("%s%s",target,document_root);
    strcat(link, document_root);
    strcat(link,"/");
    strcat(link,target);
    //printf("%s",link);
    FILE *f = fopen(link,"r");
    if(f == NULL) {
        perror("erreur fichier n'existe pas");
        exit(1);
    };
    return f;
}
int main(int argc,char** argv)
{
    if(argc != 2) {
        perror("erreur nombre d'arguments");
        exit(1);
    }
    
  
   

    
    /* message bienvenue */
    char * message_bienvenue = " _______                         __                          \n/       \\                       /  |                         \n███████  |  ______    _______  _██ |_     ______    ______   \n██ |__██ | /      \\  /       |/ ██   |   /      \\  /      \\  \n██    ██<  ██████  |/███████/ ██████/   /██████  | ██████  | \n███████  | /    ██ |██      \\   ██ | __ ██ |  ██/  /    ██ | \n██ |__██ |/███████ | ██████  |  ██ |/  |██ |      /███████ | \n██    ██/ ██    ██ |/     ██/   ██  ██/ ██ |      ██    ██ | \n███████/   ███████/ ███████/     ████/  ██_/       ███████/  \n\n\n\n";
    
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
    if(f == NULL){
	    perror ("descripteur fichier socket client");
	    exit(1);
    }
    char* buffer = malloc(sizeof(char)*TAILLE_MAX);
    fgets_or_exit(buffer,TAILLE_MAX,f);
    skip_headers(f);

    /* verification methode appelee */
    http_request request; 
    int http = parse_http_request( buffer , &request );
    printf("%s\n",rewrite_target(request.target));
    FILE* file = check_and_open("Jonas",argv[1]);

    if(!http)
        send_response( f, 400 , " Bad Request" , "Bad request\r\n" );
    else if( request.method == HTTP_UNSUPPORTED )
        send_response( f , 405 , " Method Not Allowed" , "Method Not Allowed\r\n" );
    else if( file != NULL /*strcmp(request.target, "/")==0*/ )
        send_response( f , 200 , " OK " , message_bienvenue );
    else
        send_response( f , 404 , " Not Found" , "Not Found \r\n");



    fclose(f);
    exit(0);

}
