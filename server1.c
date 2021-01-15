#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
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
    // If port number is not passed as argumrnt
    if(argc<2)
    {
        fprintf(stderr,"Port No not providded. Program terminated\n");
        exit(1);
    }

    // Decalring socket variables
    int sockfd, newsockfd, PORT, n;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    
    // Do while loop is used to ensure that server is ready to accept a client
    do
    {   
        // Creating the socket and verifying that it is created
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0)
        {
            printf("Error opening Socket!\n");
            exit(1);
        }
        else
        {
            printf("Socket Created successfully!\n");
        }
        bzero((char *)&serv_addr, sizeof(serv_addr));

        // Intitialising the variables
        PORT = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORT);

        // Binding the newly created socket to given IP
        if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("Binding failed!\n");
            exit(1);
        }
        else
        {
            printf("Socket binded successfully!\n");
        }

        // Server ready to listen
        if(listen(sockfd, 5)!=0)
        {
            printf("Listen Failed!");
            exit(1);
        }
        else
        {
            printf("Server Listening!\n");
        }

        // Accepting the connection from client
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if(newsockfd < 0)
        {
            printf("Error in accepting!\n");
            exit(1);
        }
        else 
        {
            printf("Server accepted the Client!\n\n");
        }

        // Closing the socket so that no other client is able to connect to the server
        close(sockfd);
        while(1)
        {   
            
            bzero(buffer, 255);

            //Readind message from the client
            n = read(newsockfd, buffer, 255);

            // If client unable to read client press ctrl+c then break
            if(n <= 0)
            {
                printf("Error on reading\n");
                break;
            }
            printf("Client's Message: %s", buffer);

            //Evaluating the arithmetic expression
            int result= calculate(buffer);
            char ans[255];

            // Changing integer result to string
            snprintf(ans,255,"%d",result);

            printf("Sending Reply to the client: %s\n\n", ans);
            n = write(newsockfd, ans, strlen(ans));
            if(n < 0)
            {
                printf("Error on writing\n");
            } 
        }
        printf("CONNECTION CLOSED\n\n");
        close(newsockfd);
    }while(1);
    return 0;
}