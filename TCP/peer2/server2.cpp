#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <string.h> 
#include <fcntl.h> 
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    int sock, connected, bytes_recieved,recv_data_int=0; //, true = 1;  
    char send_data [1024] , recv_data[1024];       

    struct sockaddr_in server_addr,client_addr;    
    unsigned int sin_size;

                int upload;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }


    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(5002);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(server_addr.sin_zero),8); 

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
            == -1) {
        perror("Unable to bind");
        exit(1);
    }

    if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
    }
    int cd;
    char fbuf[1024]="",start[1024]="",buf[1024]="";char copy[1024]="",buf2[1024]="",buf3[1024]="",buf4[1024]="",buf5[1024]="",forsbuf[1024]="";
    char file_size[1024];
    printf("TCPServer Waiting for client on port 5002---\n");
    fflush(stdout);

    ifstream size,time,date,md5;
    string line,line1,line2,line3;

    while(1)
    {  
        sin_size = sizeof(struct sockaddr_in);

        connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size );

        printf("I got a connection from (%s , %d)\n",
                inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        while (1)
        {
            char fbuf[1024]="",start[1024]="",buf[1024]="", copy[1024]="",buf2[1024]="",buf3[1024]="",buf4[1024]="",buf5[1024]="",forsbuf[1024]="", file_size[1024];

            // File size
            char start1[1024]="";
            bytes_recieved = read(connected,start,1024);
            start[bytes_recieved] = '\0';
            strcpy(start1,start);
            //printf("Start:%s\n", start1);
            if(!strcmp(start1, "IndexGet"))
            {
                bytes_recieved = read(connected,buf,1024);
                buf[bytes_recieved] = '\0';
               // printf("seconf: %s\n", buf);
                if(!strcmp(buf, "LongList"))
                {
                  //  printf("In\n");
                    char len[1024];int l;
                    system("ls -l | wc -l > tot");
                    ifstream tot,lof1;
                    tot.open("tot");
                    getline(tot, line);
                    tot.close();remove("tot");
                    l = atoi(line.c_str()) - 1;

                    write(connected, &l, sizeof(int));
                    char comm[1024];
                    system("touch lof");
                    sprintf(comm, "ls -l |  awk '{ print $8 }' | tail -%d > lof", l);
                    system(comm);
                    lof1.open("lof");
                    string newline[l];
                    for(int i=0;i<l;i++)
                    {
                        getline(lof1, newline[i]);
                    }
                    lof1.close();

                   // write(connected, &l, sizeof(int));
                    for(int i=0;i<l;i++)
                    {
                    //    printf("\n\n");
                        char file[1024]="";strcpy(file, newline[i].c_str());
                        if(!strcmp(file,"lof"))
                        {//nothing
                            continue;
                        }
                        else
                        {
                      //      printf("Name of file: %s", file);
                            write(connected, file, 1024);
                            strcpy(copy, file);

                            char comm1[1024];
                            sprintf(comm1, "ls -l %s | awk '{print $5}' > size", copy);
                            system(comm1);
                            size.open("size");
                            getline(size, line1);
                            size.close();remove("size");
                            write(connected, line1.c_str(), 1024);

                            char comm2[1024];
                            sprintf(comm2, "ls -l %s | awk '{print $6}' > date", copy);
                            system(comm2);
                            date.open("date");
                            getline(date, line2);
                            date.close();remove("date");
                            write(connected, line2.c_str(), 1024);

                            char comm3[1024];
                            sprintf(comm3, "ls -l %s | awk '{print $7}' > time", copy);
                            system(comm3);
                            time.open("time");
                            getline(time, line3);
                            time.close();remove("time");
                            write(connected, line3.c_str(), 1024);

                        }
                    }
                    remove("lof");
                }
                else if(!strcmp(buf, "ShortList"))
                {
                    char t1[1024],t2[1024],t11[1024],t22[1024];
                    char len[1024];int l;
                    system("ls -l | wc -l > tot");
                    ifstream tot,lof1;
                    tot.open("tot");
                    getline(tot, line);
                    tot.close();remove("tot");
                    l = atoi(line.c_str()) - 1;
                    bytes_recieved = read(connected, t1 ,1024);
                    t1[bytes_recieved] = '\0';
                    //printf("first t1: %s\n",t1);
                    strcpy(t11,t1);
                    bytes_recieved = read(connected, t2 ,1024);
                    t2[bytes_recieved] = '\0';
                    //printf("first t2: %s\n",t2);
                    strcpy(t22,t2);

                    write(connected, &l, sizeof(int));
                    char comm[1024];
                    system("touch lof");
                    sprintf(comm, "ls -l |  awk '{ print $8 }' | tail -%d > lof", l);
                    system(comm);
                    lof1.open("lof");
                    string newline[l];
                    for(int i=0;i<l;i++)
                    {
                        getline(lof1, newline[i]);
                    }
                    lof1.close();

                    // write(connected, &l, sizeof(int));
                    for(int i=0;i<l;i++)
                    {
                        //printf("\n\n");
                        char file[1024]="";strcpy(file, newline[i].c_str());
                        char comm2[1024];
                        sprintf(comm2, "ls -l %s | awk '{print $6}' > date", file);
                        system(comm2);
                        date.open("date");
                        getline(date, line2);
                        date.close();remove("date");
                        //write(connected, line2.c_str(), 1024);
                        //printf("T1: %s T2: %s date: %s\n", t11,t22, line2.c_str());
                        if(!strcmp(file,"lof") || line2 < t11 || line2 > t22)
                        {//nothingi
                            write(connected, "No", 1024);
                            continue;
                        }
                        else
                        {
                          ///  printf("Name of file: %s", file);
                            write(connected, file, 1024);
                            strcpy(copy, file);

                            char comm1[1024];
                            sprintf(comm1, "ls -l %s | awk '{print $5}' > size", copy);
                            system(comm1);
                            size.open("size");
                            getline(size, line1);
                            size.close();remove("size");
                            write(connected, line1.c_str(), 1024);


                            /*char comm3[1024];
                              sprintf(comm3, "ls -l %s | awk '{print $7}' > time", copy);
                              system(comm3);
                              time.open("time");
                              getline(time, line3);
                              time.close();remove("time");
                              write(connected, line3.c_str(), 1024);*/
                        }
                    }
                    remove("lof");
                }
                else if(!strcmp(buf, "RegEx"))
                {
                    char regex[1024];
                    bytes_recieved = read(connected, regex ,1024);
                    regex[bytes_recieved] = '\0';
                    
                    int l;char comm[1024],comm1[1024];
                    sprintf(comm, "ls -l %s | awk '{ print $8}' | wc -l > l", regex);
                    system(comm);
                    date.open("l");
                    getline(date,line);
                    l = atoi(line.c_str());
                    date.close();remove("l");
                    strcpy(copy,regex);

                    sprintf(comm1, "ls -l %s | awk '{ print $8}' > lof", copy);
                    system(comm1);
                    ifstream lof;
                    string newline[l];
                    lof.open("lof");
                    for(int i=0;i<l;i++)
                    {
                        getline(lof, newline[i]);
                    }
                    lof.close();remove("lof");
                    write(connected, &l, sizeof(int));

                    for(int i=0;i<l;i++)
                    {
                            char comm2[1024];
                           //printf("Name of file: %s\n", newline[i].c_str());
                           write(connected, newline[i].c_str(), 1024);
                           sprintf(comm2, "ls %s -l | awk '{ print $5 }' > size", newline[i].c_str());
                           system(comm2);
                           size.open("size");
                           getline(size,line2);
                           size.close();remove("size");
                           //printf("Size of file: %s\n", line2.c_str());
                           write(connected, line2.c_str(), 1024);
                    }

                    
                }
                else
                {
                    //printf("Inavlid Arguements.\n");
                }

            }
            else if(!strcmp(start1, "FileHash"))
            {
                char ver[1024]="";
                bytes_recieved = read(connected,ver,1024);
                ver[bytes_recieved] = '\0';
        //        printf("Itis: %s\n", ver);
                if(!strcmp(ver,"Verify"))
                {
                    bytes_recieved = read(connected, buf2,1024);
                    buf2[bytes_recieved] = '\0';
          //          printf("--%s\n", buf2);
                    strcpy(copy,buf2);
                    FILE* fd;
                    if((fd = fopen(copy,"r")) == NULL)
                    {
                      //  printf("\nNo such file exits\n");
                        write(connected, "Error", 1024);
                    }
                    else
                    {
                        //fclose(fd);
                        char comm3[1024];
                        sprintf(comm3, "md5sum %s | awk '{print $1}' > md5",copy); 
                        system(comm3);
                        md5.open("md5");
                        getline(md5,line3);
                        strcpy(buf5,line3.c_str());
                        write(connected, buf5, 1024);
                        
                        //printf("md5--:%s\n",buf5);
                        
                        md5.close();
                        remove("md5");
                        // Date
                        char comm1[1024],comm2[1024];
                        sprintf(comm1, "ls -al %s | awk '{print $6 }' > date",copy);
                        sprintf(comm2, "ls -al %s | awk '{print $7 }' > time",copy);
                        system(comm1);system(comm2);
                        date.open("date");
                        getline(date,line1);
                        strcpy(buf3,line1.c_str());
                        write(connected, buf3, 1024);
                        date.close();
                        remove("date");
                        
                        //printf("date--:%s\n",buf3); 
                        
                        // Time
                        time.open("time");
                        getline(time,line2);
                        strcpy(buf4,line2.c_str());
                        write(connected, buf4, 1024);
                        time.close();
                        remove("time");
                        
                        //printf("time--:%s\n",buf4); 

                        // If not the same md5 the to correct :P
                        char chek[1024];
                        bytes_recieved = read(connected, chek,1024);
                        chek[bytes_recieved] = '\0';
                        if(!strcmp(chek,"OK"))
                        {
                        //    printf("File is up-to-date.\n");
                        }
                        else if(!strcmp(chek,"NOK"))
                        {
                            //FILE* fd;
                            //fd = fopen(copy,"r");
                            char c;int count=0;char data[1024];
                            while(fscanf(fd, "%c" , &c)!=EOF)
                            {
                                count=0;
                                data[count++] = c;
                                while(count < 1024 && fscanf(fd, "%c" , &c)!=EOF)
                                    data[count++]=c;
                          //      printf("part x: %s and count %d\n", data, count);
                                write(connected, &count, sizeof(int));
                                write(connected, data, 1024);
                                //         printf("---------------------------------------------------------------------------------------------------------------\n");
                            }
                            //       printf("---------------------------------------------------------------------------------------------------------------\n");
                            write(connected, &recv_data_int, sizeof(int));
                            write(connected, "Over", 1024);
                            fclose(fd);

                        }
                        else
                        {
                        //    printf("Some Error\n");
                        }

                    }
                }
                else if(!strcmp(ver,"CheckAll"))
                {
                    read(connected ,&recv_data_int,sizeof(int));
                    char file[1024];
            //        printf("limit is:%d\n", recv_data_int);
                    for(int i=0;i<recv_data_int;i++)
                    {
                        bytes_recieved = read(connected ,file ,1024);
                        file[bytes_recieved] = '\0';
                        strcpy(copy, file);
                        FILE* fd;
                        if((fd = fopen(file ,"r")) == NULL)
                        {
                        //    printf("\nNo such file like %s exits\n", file);
                            write(connected, "Error", 1024);
                        }
                        else
                        {
                          //  printf("File name is %s\n", copy);
                            //md5 and time etc..    
                            char comm3[1024];
                            sprintf(comm3, "md5sum %s | awk '{print $1}' > md5", copy); 
                            system(comm3);
                            md5.open("md5");
                            getline(md5,line3);
                            strcpy(buf5,line3.c_str());
                            write(connected, buf5, 1024);
                            //printf("md5--:%s\n",buf5);
                            md5.close();
                            remove("md5");
                            // Date
                            char comm1[1024],comm2[1024];
                            sprintf(comm1, "ls -al %s | awk '{print $6 }' > date", copy);
                            sprintf(comm2, "ls -al %s | awk '{print $7 }' > time", copy);
                            system(comm1);system(comm2);
                            date.open("date");
                            getline(date,line1);
                            strcpy(buf3,line1.c_str());
                            write(connected, buf3, 1024);
                            date.close();
                            remove("date");
                            //printf("date--:%s\n",buf3); 
                            // Time
                            time.open("time");
                            getline(time,line2);
                            strcpy(buf4,line2.c_str());
                            write(connected, buf4, 1024);
                            time.close();
                            remove("time");
                            //printf("time--:%s\n",buf4);
                            // 
                            char chek[1024];
                            bytes_recieved = read(connected, chek,1024);
                            chek[bytes_recieved] = '\0';
                            if(!strcmp(chek,"OK"))
                            {
                            //    printf("File is up-to-date.\n");
                            }
                            else if(!strcmp(chek,"NOK"))
                            {
                                //FILE* fd1;
                                //fd1 = fopen(copy,"r");
                                char c;int count;char data[1024]="";int x=0;
                                while(fscanf(fd, "%c" , &c)!=EOF)
                                {
                                    count=0;
                                    data[count++] = c;
                                    while(count < 1024 && fscanf(fd, "%c" , &c)!=EOF)
                                        data[count++]=c;
                              //      printf("part x: %s and count %d\n", data, count);
                                    write(connected, &count, sizeof(int));
                                    write(connected, data, 1024);
                                }
                                write(connected, &count, sizeof(int));
                                write(connected, "Over", 1024);
                                fclose(fd);

                            }
                            else
                            {
                             //   printf("Some Error\n");

                            }
                        }

                    }
                }
                else
                {
                //    printf("Some error in the request by the client\n");
                }

            }
            else if(!strcmp(start1,"FileUpload"))
            {

             //   printf("A request from uploading a file from the client:\nType: \nFileUpload Allow: to allow.\nFileUpload Deny: to deny.\n");
                //---------------------REPLACE--------------------gets(buf);
                read(connected, &upload, sizeof(int));
//                printf("nvjnfbvjf: %d\n ", upload);
                if(upload == 0)
                {
                    printf("Allowing to upload..............\n"); 
                    strcpy(buf, "Allow");
                }
                else
                {
                    printf("Denying to upload.........\n");
                    strcpy(buf, "Deny");
                }
                write(connected, buf, 1024);
               // printf("so it is:--%s\n", buf);
                if(!strcmp(buf,"Allow"))
                {
                    bytes_recieved = read(connected,buf2,1024);
                    buf2[bytes_recieved] = '\0';
                    strcpy(copy,buf2);
                //    printf("Name of File Uploaded: %s\n",buf2);

                    bytes_recieved = read(connected,forsbuf,1024);
                    forsbuf[bytes_recieved] = '\0';
                  //  printf("Size of that file: %s \n", forsbuf);

                    bytes_recieved = read(connected,buf5,1024);
                    buf5[bytes_recieved] = '\0';
                  //  printf("MD5 hash of that file: %s \n", buf5);

                    bytes_recieved = read(connected,buf3,1024);
                    buf3[bytes_recieved] = '\0';
                  //  printf("Last Modified Date: %s\n", buf3);

                    bytes_recieved = read(connected,buf4,1024);
                    buf4[bytes_recieved] = '\0';
                  //  printf("Last Modified Time: %s \n",buf4);
                    // File


                    read(connected , &recv_data_int,sizeof(int));
                    bytes_recieved = read(connected, fbuf ,1024);
                    fbuf[bytes_recieved]='\0';

                    FILE *fd;
                    fd = fopen(copy,"w");
                    while(strcmp(fbuf,"Over")!=0)
                    {
                        for(int i=0;i<recv_data_int;i++)
                            fprintf(fd,"%c",fbuf[i]);
                        read(connected ,&recv_data_int,sizeof(int));
                        bytes_recieved = read(connected, fbuf, 1024);
                        fbuf[bytes_recieved]='\0';
                    //    printf("string by parts id %s and %d\n", fbuf, recv_data_int);
                    }
                    fclose(fd);

                }
                else if(!strcmp(buf, "Deny"))
                {
                  //  printf("Server has disallowed the uploading of the file.\n");        
                }
                else
                {
                   // printf("Error.\n");
                }

            }
            else if(!strcmp(start1,"FileDownload"))
            {
            //    printf("jjjj");
                bytes_recieved = read(connected,buf,1024);
                buf[bytes_recieved] = '\0';
              //  printf("File is :%s", buf);
                strcpy(copy,buf);
                FILE* fd;
                if((fd = fopen(copy,"r")) == NULL)
                {
               //     printf("\nNo such file exits\n");
                    write(connected, "Error", 1024);
                }
                else
                {
                    char comm[1024];
                    sprintf(comm, "ls -al %s | awk '{print $5}' > size", buf);
                    system(comm);
                    size.open("size");
                    getline(size,line);
                    strcpy(forsbuf,line.c_str());
                    write(connected, forsbuf,strlen(forsbuf));  
                 //   printf("size :%s\n",forsbuf); 
                    size.close();
                    remove("size");
                    // File
                    //strcpy(buf2,copy);
                    char c;int count = 0;int x=0,c2=0;
                    /*while(1)
                      {
                      c2 = 0;
                      if(count == atoi(forsbuf))
                      break;
                      fscanf(fd, "%c", &c);
                      buf2[c2++] = c;
                      while(c2 < 1024 && fscanf(fd, "%c", &c)!=EOF)
                      buf2[c2++] = c;
                      write(connected, &c2, sizeof(int));
                      write(connected, buf2, 1024);
                      count++;
                      }
                      write(connected, &x, sizeof(int));
                      write(connected, "Over", 1024);*/
                    while(fscanf(fd, "%c" , &c)!=EOF)
                    {   
                        count=0;
                        buf2[count++] = c;
                        while(count < 1024 && fscanf(fd, "%c" , &c)!=EOF)
                            buf2[count++]=c;
                   //     printf("part x: %s and count %d\n", buf2, count);
                        write(connected, &count, sizeof(int));
                        write(connected, buf2, 1024);
                        //         printf("---------------------------------------------------------------------------------------------------------------\n");
                    }
                    //       printf("---------------------------------------------------------------------------------------------------------------\n");
                    write(connected, &x, sizeof(int));
                    write(connected, "Over", 1024);
                    fclose(fd);


                    //int fd=open(buf2,O_RDONLY);
                    //if(atoi(forsbuf)>1024)
                    //{ 

                    //        bytes_recieved = read(fd, buf2, 1024 ); 
                    //      buf2[bytes_recieved] = '\0';
                    //    write(connected,buf2,strlen(buf2));
                    //  printf("BUF2--:%s\n",buf2);

                    //}
                    // MD5 hash value
                    char comm3[1024];
                    sprintf(comm3, "md5sum %s | awk '{print $1}' > md5",copy); 
                    system(comm3);
                    md5.open("md5");
                    getline(md5,line3);
                    strcpy(buf5,line3.c_str());
                    write(connected, buf5, 1024);
                   // printf("md5--:%s\n",buf5);
                    md5.close();
                    remove("md5");
                    // Date
                    char comm1[1024],comm2[1024];
                    sprintf(comm1, "ls -al %s | awk '{print $6 }' > date",copy);
                    sprintf(comm2, "ls -al %s | awk '{print $7 }' > time",copy);
                    system(comm1);system(comm2);
                    date.open("date");
                    getline(date,line1);
                    strcpy(buf3,line1.c_str());
                    write(connected, buf3, 1024);
                    date.close();
                    remove("date");
                  //  printf("date--:%s\n",buf3); 
                    // Time
                    time.open("time");
                    getline(time,line2);
                    strcpy(buf4,line2.c_str());
                    write(connected, buf4, 1024);
                    time.close();
                    remove("time");
                    //printf("time--:%s\n",buf4); 
                } 
            
            }
            fflush(stdout);
        } 
                    close(connected);
    }
    close(sock);
    return 0;
} 
