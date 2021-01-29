/******************************************

    Shell Code

    Disciplina: Sistemas Operacionais
    
    Autor: Paulo José Carmona Teixeira
    Matricula: 11611ECP018

    Data: 01/07/2019

******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio_ext.h>
#include <string.h>
#include <limits.h>

#define BWHITE      "\x1b[97m"
#define GREEN       "\x1b[32m"
#define BGREEN      "\x1b[92m"
#define BBLUE       "\x1b[94m"
#define RST_COLOR   "\x1b[0m"



/*   Declaração das Funções   */

char ** array_arrange_path(char * text, char * path);

char ** array_parsing(char * text);

void cd_in(char * args);

void cd_out();

char * current_dir();



/*   Inicialização da Main   */

int main(int argc, char *argv[])
{
    char text_shell[100];
    char **args_path, **args, *user, *dir;
    char userPC[HOST_NAME_MAX];

    // Declaração dos paths das pastas de comandos padrão

    char * path1 = "/usr/local/bin/";
    char * path2 = "/usr/bin/";
    char * path3 = "/bin/";
    char * path4 = "/usr/local/sbin/";
    char * path5 = "/usr/sbin/";
    char * path6 = "/sbin/"; 

    user = getenv("USER");
    gethostname(userPC, HOST_NAME_MAX);

    while(1)
    {
        dir = current_dir();
        __fpurge(stdin); /* ou fflush(stdin) */
        fprintf(stdout, BGREEN "[%s@%s " RST_COLOR, user, userPC);
        fprintf(stdout, BWHITE "%s " RST_COLOR, dir);
        fprintf(stdout, BGREEN "]$ " RST_COLOR);
        fprintf(stdout, RST_COLOR);
        fgets(text_shell,100,stdin);
        fprintf(stdout, BBLUE);

        args = array_parsing(text_shell);

        if(!strcmp(args[0], "exit") || !strcmp(args[0], "cd"))
        {
            if(!strcmp(args[0], "exit"))
            {
                printf("\n Exit\n");
                exit(0);
                return(0);
            }
            else
            {
                if(args[1] == NULL)
                {

                }
                else
                {
                    if(!strcmp(args[1], ".."))
                    {
                        cd_out();
                    }
                    else
                    {
                        cd_in(args[1]);
                    }
                }
            }
        }
        else
        {
            pid_t process_ID = fork(); // cria o processo filho e armazena o PID retornado da função fork
            int status;

            if(process_ID < 0) // <0 retorna erro
            {
                fprintf(stderr, "%s\n", "Error\n");
                status = -1;
                _exit(EXIT_FAILURE);
            }
            else if (process_ID != 0) // Caso fork() retorne o PID do filho, ele é o processo pai
            {
                waitpid(process_ID, NULL, 0); // espera até o processo filho terminar
            }              
            else
            {
                    // Executa as funcões padrão do OS

                args_path = array_arrange_path(text_shell,path1);
                execvp(args_path[0],args_path);

                args_path = array_arrange_path(text_shell,path2);
                execvp(args_path[0],args_path);

                args_path = array_arrange_path(text_shell,path3);
                execvp(args_path[0],args_path);

                args_path = array_arrange_path(text_shell,path4);
                execvp(args_path[0],args_path);

                args_path = array_arrange_path(text_shell,path5);
                execvp(args_path[0],args_path);

                args_path = array_arrange_path(text_shell,path6);
                execvp(args_path[0],args_path);

                fprintf(stdout,"bash: %s command not found\n",args[0]);
            }            
        }        
    }

    return 0;
}



/*   Definição das Funções   */

char ** array_arrange_path(char * text, char * path)
{
    char **args = NULL;    
    int i, num_args = 0, size_arg = strlen(path);    
    args = malloc (sizeof (char*));
    args[num_args] = malloc (sizeof (char));
    args[num_args] = realloc (args[num_args],(sizeof(char)*(size_arg)));
    for (int i = 0; i < strlen(path); ++i)
    {
        args[num_args][i] = path[i];
    }      

    for(i=0;i < 100 && text[i] != '\n';i++)
    {        
        if(text[i] == ' ')
        {            
            if(args[0][0] == 0x00)
            {
                continue;
            }
            else
            {            
                args[num_args][size_arg] = 0x00;
                args = realloc (args,(sizeof(char*)*(num_args+1)));                
                num_args++;
                size_arg = 0;                             
            }            
        }
        else
        {
            args[num_args] = realloc (args[num_args],(sizeof(char)*(size_arg+2)));                            
            args[num_args][size_arg] = text[i];
            size_arg++;
        }       
    }
    args[num_args][size_arg] = 0x00;
    return args;
}

char ** array_parsing(char * text)
{
    char **args = NULL;    
    int i, num_args = 0, size_arg = 0;    
    args = malloc (sizeof (char*));
    args[num_args] = malloc (sizeof (char));    

    for(i=0;i < 100 && text[i] != '\n';i++)
    {        
        if(text[i] == ' ')
        {            
            if(args[0][0] == 0x00)
            {
                continue;
            }
            else
            {            
                args[num_args][size_arg] = 0x00;
                args = realloc (args,(sizeof(char*)*(num_args+1)));                
                num_args++;
                size_arg = 0;                             
            }            
        }
        else
        {
            args[num_args] = realloc (args[num_args],(sizeof(char)*(size_arg+2)));                            
            args[num_args][size_arg] = text[i];
            size_arg++;
        }       
    }
    args[num_args][size_arg] = 0x00;
    return args;
}

void cd_in(char * args)
{
    long size;
    char *buf, *ptr;
    size = pathconf(".", _PC_PATH_MAX);
    if ((buf = (char *)malloc((size_t)size)) != NULL)
        ptr = getcwd(buf, (size_t)size);

    ptr = strcat(ptr,"/");
    ptr = strcat(ptr,args);

    chdir(ptr); 
}

void cd_out()
{
    long size;
    char *buf, *ptr;
    size = pathconf(".", _PC_PATH_MAX);
    if ((buf = (char *)malloc((size_t)size)) != NULL)
        ptr = getcwd(buf, (size_t)size);

    int i,tam = strlen(ptr);

    for (int i = tam; i >= 0; i--)
    {
        if(ptr[i] == '/')
        {
            ptr[i] = 0x00;
            break;
        }
        else
        {
            ptr[i] = 0x00;
        }        
    }
    chdir(ptr);
}

char * current_dir()
{        
    char cwd[1024],*dir = NULL;
    getcwd(cwd, sizeof(cwd));

    int i,tam = strlen(cwd),j=0;

    dir = malloc (sizeof (char));

    for(i=tam;i >=0;i--)
    {
        if(cwd[i] == '/')
        {            
            break;
        }
    }
    tam = i + 1;    

    for (int i = tam; i >= 0; i++)
    {
        if(cwd[i] == 0x00)
        {
            dir[j] = 0x00;
            break;
        }
        else
        {            
            dir = realloc (dir,(sizeof(char)*(j+2)));                                     
            dir[j] = cwd[i];      
            j++;
        }               
    }
    return dir;      
}