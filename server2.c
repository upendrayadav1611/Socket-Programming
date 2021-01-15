#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<netdb.h>

// Logic to evaluate the expression
int calculate(char *msg)
{
    int n=strlen(msg);
    int num1=0,num2=0,i;
    int ok1=0,ok2=0;
    int flag1=0;

    // Logic to obtain first operand
    for(i=0;msg[i]!='\n';i++)
    {   
        
        if(flag1==1&&(msg[i]=='+'||msg[i]=='-'||msg[i]=='*'||msg[i]=='/'))
        break;
        else if(msg[i]=='-')
        ok1=1;
        else if(msg[i]!=' ')
        {
            num1 = num1*10+(msg[i]-'0');
            flag1=1;
        }
    }

    // opr stores the operator
    char opr = msg[i];
    i++;

    //Logic to obtain the second operand
    for(;msg[i]!='\n';i++)
    {
        if(msg[i]=='-')
        ok2=1;
        else if(msg[i]!=' ')
        num2=num2*10+(msg[i]-'0');
    }

    // If operands are negative
    if(ok1==1)
    num1=num1*(-1);
    if(ok2==1)
    num2=num2*(-1);

    //Calculation
    if(opr=='+')
    return num1+num2;
    else if(opr=='-')
    return num1-num2;
    else if(opr=='*')
    return num1*num2;
    else
    return num1/num2;
}
int main(int argc, char *argv[])
{
    if(argc<2)
    {
        fprintf(stderr,"Port No not providded. Program terminated\n");
        exit(1);
    }

    // Intitialising Socket variables
    int sockfd, newsockfd, PORT, n;
    socklen_t addr_size;
    pid_t childpid;
    struct sockaddr_in serv_addr, cli_addr;
    
    char buffer[255];

    // Socket Creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("Error opening Socket!\n");
        exit(1);
    }
    else
    {
        printf("Socket Created Successfully!\n");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));

    // Intitialising the variables
    PORT = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Binding failed!\n");
        exit(1);
    }
    else
    {
        printf("Socket binded Successfully!\n");
    }
    if(listen(sockfd, 10)!=0)
    {
        printf("Listen failed!\n");
        exit(1);
    }
    else
    {
        printf("Server Listening!\n");
    }
    while(1){
        addr_size = sizeof(cli_addr);

        //Accepting the Clients
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_size);
        if(newsockfd < 0)
        {
            printf("Error on accept or Client pressed Ctrl+C\n\n");
            exit(1);
        }
        printf("Connection accepted from %s:%d\n",inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        // Logic to handle multiple client using fork
        if((childpid=fork())==0)
        {
            close(sockfd);

            while(1)
            {
                // Reading Client's message
                bzero(buffer,255);
                n= read(newsockfd,buffer,255);
                if(n<=0)
                {
                    printf("Disconneted from %s:%d\n",inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                    break;
                }
                printf("Client's message: %s", buffer);

                // Evaluating the expression
                int result= calculate(buffer);
                char ans[255];

                // Conerting the result into string and sending back to client
                snprintf(ans,255,"%d",result);
                printf("Sending reply to the client: %s\n\n",ans);
                write(newsockfd, ans, strlen(ans));
            }
        }
    }
    // Closing the socket
    close(sockfd);
    return 0;
}