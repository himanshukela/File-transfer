#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <signal.h>
using namespace std;
int ex = 0, upload = 0;
void serverclose()
{
    system("ps aux | grep ./runner | head -1 | awk '{ print $2 }' > pid");
    ifstream pid;
    string pp; 
    pid.open("pid");
    getline(pid,pp);
    printf("Port: %s closed\n ",pp.c_str());
    pid.close();remove("pid");
    //int p = atoi(pp.c_str());
    //printf("PID is :%d\n", p);
    char sss[1024];
    sprintf(sss,"kill -9 %s",pp.c_str());
    system(sss);

}
void check()
{
    system("ps aux | grep ./client | wc -l > check");
    ifstream pid;
    string pp; 
    pid.open("check");
    getline(pid,pp);
    //printf("--%s--\n ",pp.c_str());
    pid.close();remove("check");
    //int p = atoi(pp.c_str());
    //printf("PID is :%d\n", p);
    int x=atoi(pp.c_str());
    if(x < 2)
        serverclose();
    //char sss[1024];
    //sprintf(sss,"kill -9 %s",pp.c_str());
    //system(sss);

}

void sigHandler(int sig)
{
    ex = 1;
    printf("Found a Ctrl-C\n");
//    signal(sig, SIG_IGN);
}
int main()
{
    system("./runner &");
/*    int type;
    printf("Type 0 for tcp and 1 for upd connection:\n");
    scanf("%d",&type);
  */  
    int sock, bytes_recieved;  
    char send_data[1024],recv_data[1024];
    struct hostent *host;
    struct sockaddr_in server_addr;  

    host = gethostbyname("127.0.0.1");

    //if(type == 0)
    //{
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
   // }
   /* else
    {
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
    }*/

    server_addr.sin_family = AF_INET;     
    server_addr.sin_port = htons(5002);   
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8); 
    int recv_data_int;
    char ver[1024]="",fbuf[1024]="",buf[1024]="",buf2[1024]="", buf3[1024]="",buf4[1024]="",buf5[1024]="", copy[1024]="",start[1024]="",forsbuf[1024]="",comm[1024]="";
    ofstream in;
    ifstream size, date, time, md5;
    string line,line1,line2,line3;
    int ff=0,cl=0;
    while(1)
    {
        char ver[1024]="",fbuf[1024]="",buf[1024]="",buf2[1024]="", buf3[1024]="",buf4[1024]="",buf5[1024]="", copy[1024]="",start[1024]="",forsbuf[1024]="",comm[1024]="";
        // signal(SIGINT, sigHandler);
        //if(type == 0)
        //{
            if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) 
            {
                //    printf("doesnt cnnect");
                //  perror("Connect");
                // exit(1);
            }
            else{
                ff = 1;
            }
        //}
        //else 
        //    ff = 1;
        if(ff == 1)
        {
            //           cl = 1;
            printf("ENTER THE MESSAGE(type q or Q to quit):\n"); 
            check();
            scanf("%s",start);
            if(!strcmp(start,"q") || !strcmp(start,"Q"))        
            {
                close(sock);
                serverclose();
                /*system("ps aux | grep ./server | head -1 | awk '{ print $2 }' > pid");
                ifstream pid;string pp;
                pid.open("pid");
                getline(pid,pp);
                pid.close();remove("pid");
                //int p = atoi(pp.c_str());
                //printf("PID is :%d\n", p);
                char sss[1024];
                sprintf(sss,"kill -9 %s",pp.c_str());
                system(sss);*/
                exit(0);
            }
            else if(!strcmp(start,"Allow"))
            {
                upload = 0;
            }
            else if(!strcmp(start,"Deny"))
            {
                upload = 1;
            }
            else if(!strcmp(start,"IndexGet"))
            {
                write(sock, start, 1024);
                scanf("%s", buf);
                write(sock, buf, 1024);
                if(!strcmp(buf, "LongList"))
                {
                    read(sock, &recv_data_int, 1024);
                    for(int i=0;i<recv_data_int-1;i++)
                    {
                        char file[1024];
                        bytes_recieved = read(sock, file, 1024);
                        file[bytes_recieved]='\0';
                        printf("Name of file: %s\n", file);

                        bytes_recieved = read(sock, file, 1024);
                        file[bytes_recieved]='\0';
                        printf("Size of file: %s\n", file);

                        bytes_recieved = read(sock, file, 1024);
                        file[bytes_recieved]='\0';
                        printf("Last Modified Date of file: %s\n", file);

                        bytes_recieved = read(sock, file, 1024);
                        file[bytes_recieved]='\0';
                        printf("Last Modified Time of file: %s\n", file);
                    }

                }
                else if(!strcmp(buf,"ShortList"))
                {
                    char t1[1024],t2[1024];
                    scanf("%s",t1);
                    write(sock, t1, 1024);
                    scanf("%s",t2);
                    write(sock, t2, 1024);
                    read(sock, &recv_data_int, 1024);
                    for(int i=0;i<recv_data_int;i++)
                    {
                        char file[1024];
                        bytes_recieved = read(sock, file, 1024);
                        file[bytes_recieved]='\0';
                        if(!strcmp(file, "No"))
                        {
                            continue;
                        }
                        else
                        {
                            printf("Name of file: %s\n", file);
                            bytes_recieved = read(sock, file, 1024);
                            file[bytes_recieved]='\0';
                            printf("Size of file: %s\n", file);

                        }


                    }
                }
                else if(!strcmp(buf,"RegEx"))
                {
                    char regex[1024];
                    scanf("%s", regex);
                    write(sock, regex, 1024);

                    read(sock, &recv_data_int, sizeof(int));
                    for(int i=0;i<recv_data_int;i++)
                    {
                        char file[1024];
                        bytes_recieved = read(sock, file, 1024);
                        file[bytes_recieved]='\0';
                        printf("Name of file: %s\n", file);

                        bytes_recieved = read(sock, file, 1024);
                        file[bytes_recieved]='\0';
                        printf("Size of file: %s\n", file);
                    }


                }
                else
                {
                    printf("Invalid arguement.\n");
                }
            }
            else if(!strcmp(start,"FileHash"))
            {
                write(sock, start, 1024);
                scanf("%s", ver);
                if(!strcmp(ver,"Verify"))
                {
                    write(sock, ver, 1024);
                    scanf("%s",buf2 );
                    strcpy(copy, buf2);
                    printf("Name:%s\n",buf2);
                    write(sock , buf2, 1024);

                    bytes_recieved = read(sock, buf5, 1024);
                    buf5[bytes_recieved]='\0';
                    if(!strcmp(buf5, "Error"))
                    {
                        printf("No such file exits in th server.\n");
                    }
                    else
                    {
                        strcpy(copy, buf2);
                        char oldmd5[1024];
                        strcpy(oldmd5, buf5);
                        printf("The MD5 value is: %s\n",oldmd5);

                        bytes_recieved = read(sock, buf3, 1024);
                        buf3[bytes_recieved]='\0';
                        printf("Date is :%s\n", buf3);

                        bytes_recieved = read(sock, buf4, 1024);
                        buf4[bytes_recieved]='\0';
                        printf("Time is :%s\n", buf4);
                        //printf("COPY is :%s and buf5 is %s and oldmd5 is %s\n", copy, buf5,oldmd5); 
                        char mymd5[1024], comm1[1024];
                        sprintf(comm1, "md5sum %s | awk '{print $1}' > md5",copy);
                        system(comm1);
                        md5.open("md5");
                        getline(md5,line3);
                        strcpy(mymd5 ,line3.c_str());
                        //write(connected, buf5, 1024);
                        printf("My md5 value is:%s\n", mymd5);
                        md5.close();
                        remove("md5");
                        //printf("CHECK---%s--%s--\n", oldmd5 , mymd5);
                        if(!strcmp(mymd5, oldmd5))
                        {
                            printf("The file is up-to-date.\n");
                            write(sock, "OK", 1024);
                        }
                        else
                        {
                            strcpy(copy, buf2);
                            printf("Need to update file %s.\n", copy);
                            write(sock, "NOK", 1024);


                            //delete and make the file again, same steps over here  
                            remove(copy);
                            char data[1024];
                            read(sock,&recv_data_int,sizeof(int));
                            bytes_recieved = read(sock,data,1024);
                            data[bytes_recieved]='\0';
                            //printf("\n");
                            //printf("---------string is : %s recv_datda_int %d--",buf2, recv_data_int);
                            FILE *fd;
                            fd = fopen(copy,"w");
                            while(strcmp(data,"Over")!=0)
                            {
                                for(int i=0;i<recv_data_int;i++)
                                    fprintf(fd,"%c",data[i]);
                                read(sock,&recv_data_int,sizeof(int));
                                bytes_recieved = read(sock,data,1024);
                                data[bytes_recieved]='\0';
                              //  printf("string by parts is %s and %d\n", data, recv_data_int);
                            }
                            fclose(fd);
                        }
                    }
                }
                else if(!strcmp(ver,"CheckAll"))
                {
                    write(sock, ver, 1024);
                    //printf("it is will: %s\n", ver );
                    char len[1024];int l;
                    system("ls -p | grep -v / | wc -l > tot");
                    ifstream tot,lof1;
                    tot.open("tot");
                    getline(tot,line);
                    tot.close();
                    remove("tot");
                    strcpy(len, line.c_str());
                    l = atoi(len);
                  //  printf("L: %d\n", l);
                    //tot.close();
                    system("touch lof");
                    sprintf(comm, "ls -p -l | grep -v / | awk '{ print $8 }' | tail -%d > lof", l);
                    system(comm);
                    lof1.open("lof");
                    write(sock, &l, sizeof(int));
                    string newline[l];
                    for(int i=0;i<l;i++)
                    {
                        getline(lof1, newline[i]);
                    }
                    lof1.close();
                    for(int i=0;i<l;i++)
                    {
                    //    printf("Lllll:  %d \n",l);
                        //getline(lof1, line1);
                      //  printf("Line is : %s\n", newline[i].c_str());
                        // now check individually the md5 hash of each file in client and update each of them
                        // first ask for the md5 hash value of each file from the server and if file is not there then best
                        // then check for the present value - if same then Good -> if not UPDATE(same as above thing)
                        char file[1024]="";strcpy(file, newline[i].c_str());
                        if(!strcmp(file,"lof"))
                        {
                        //    printf("haannnnnnnnnn\n");
                            continue;
                        }
                        else
                        {
                            write(sock, file, 1024);
                            strcpy(copy, file);
                            bytes_recieved = read(sock, buf5, 1024);
                            buf5[bytes_recieved]='\0';
                            char Md5[1024];strcpy(Md5,buf5);
                            strcpy(copy, file);
                            if(!strcmp(buf5, "Error"))
                            {
                                printf("File %s does not exits in the server.\n", copy);
                            }
                            else
                            {
                                strcpy(copy, file);
                                printf("MD5 for %s file in the server is %s\n", copy, Md5);

                                bytes_recieved = read(sock, buf3, 1024);
                                buf3[bytes_recieved]='\0';
                                printf("Date is :%s\n", buf3);

                                bytes_recieved = read(sock, buf4, 1024);
                                buf4[bytes_recieved]='\0';
                                printf("Time is :%s\n", buf4);

                                //        char Mymd5[1024];
                                char mymd5[1024], comm1[1024];
                                sprintf(comm1, "md5sum %s | awk '{print $1}' > md5",copy);
                                system(comm1);
                                md5.open("md5");
                                getline(md5,line3);
                                strcpy(mymd5 ,line3.c_str());
                                //write(connected, buf5, 1024);
                                printf("My md5 is :%s\n", mymd5);
                                md5.close();
                                remove("md5");
                                //printf("CHECK---%s--%s--\n", oldmd5 , mymd5);
                                if(!strcmp(mymd5, Md5))
                                {
                                    printf("The file is up-to-date.\n");
                                    write(sock, "OK", 1024);
                                }
                                else
                                {
                                    strcpy(copy, file);
                                    printf("Need to update file %s.\n", copy);
                                    write(sock, "NOK", 1024);


                                    //delete and make the file again, same steps over here  
                                    remove(copy);
                                    FILE *fd;
                                    fd = fopen(copy,"w");
                                    char data[1024]="";int din=0;
                                    read(sock,&din,sizeof(int));
                                    bytes_recieved = read(sock,data,1024);
                                    data[bytes_recieved]='\0';
                                    //printf("\n");
                                    //      printf("--string is : %s recv_datda_int %d--",buf2, recv_data_int);
                                    while(strcmp(data,"Over")!=0)
                                    {
                                        for(int i=0;i<din;i++)
                                            fprintf(fd,"%c",data[i]);
                                        read(sock,&din,sizeof(int));
                                        bytes_recieved = read(sock,data,1024);
                                        data[bytes_recieved]='\0';
                              //          printf("string by parts id %s and %d\n", data, din);
                                    }
                                   // printf("In-1\n");
                                    fclose(fd);
                                   // printf("In-2\n");

                                }
                            }


                        }
                    }
                    remove("lof");
                }
            }
            else if(!strcmp(start,"FileUpload"))
            {
                write(sock, start, 1024);
                scanf("%s",buf2);
                printf("Waiting for the response of the server............\n");

                write(sock, &upload, sizeof(int));
                strcpy(copy,buf2);
                bytes_recieved = read(sock, buf, 1024);
                buf[bytes_recieved]='\0';
                if(!strcmp(buf,"Allow"))
                {
                    strcpy(buf2,copy);
                    //printf("copy : %s and buf2: %s\n",copy,buf2);
                    FILE* fd;
                    if((fd = fopen(copy,"r")) == NULL)
                    {
                        printf("No such file exits\n");
                        //write(connected, "Error", 1024);
                    }
                    else
                    {
                        printf("Name of File Uploaded: %s\n",buf2);
                        write(sock, buf2, 1024);
                        sprintf(comm, "ls -al %s | awk '{print $5}' > size", copy);
                        system(comm);
                        size.open("size");
                        getline(size,line);
                        strcpy(forsbuf,line.c_str());
                        write(sock, forsbuf, 1024);  
                        size.close();
                        remove("size");
                        printf("Size of that file: %s \n",forsbuf);

                        // MD5 hash value
                        char comm3[1024];
                        sprintf(comm3, "md5sum %s | awk '{print $1}' > md5",copy);
                        system(comm3);
                        md5.open("md5");
                        getline(md5,line3);
                        strcpy(buf5,line3.c_str());
                        write(sock, buf5, 1024);
                        printf("MD5 hash of that file: %s \n",buf5);
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
                        write(sock, buf3, 1024);
                        date.close();
                        remove("date");
                        printf("Last Modified Date: %s\n",buf3);
                        // Time
                        time.open("time");
                        getline(time,line2);
                        strcpy(buf4,line2.c_str());
                        write(sock , buf4, 1024);
                        time.close();
                        remove("time");
                        printf("Last Modified Time: %s \n",buf4);

                        //Contents of the file character wise
                        char c;int count;
                        while(fscanf(fd, "%c" , &c)!=EOF)
                        {
                            count=0;
                            fbuf[count++] = c;
                            while(count < 1024 && fscanf(fd, "%c" , &c)!=EOF)
                                fbuf[count++]=c;
                         //   printf("part x: %s and count %d\n", fbuf, count);
                            write(sock, &count, sizeof(int));
                            write(sock, fbuf, 1024);
                        }
                        write(sock , &recv_data_int, sizeof(int));
                        write(sock , "Over", 1024);
                        fclose(fd);
                    }


                    //
                }
                else if(!strcmp(buf, "Deny"))
                {
                    printf("You are not permitted to upload the file.\n");
                }
                else
                {
                    printf("Some Error occured while uploading.\n");
                }
            }
            else if(!strcmp(start,"FileDownload"))
            {
                write(sock, start, 1024);
                scanf("%s",buf);
            //    printf("Response:\n");
                strcpy(copy,buf);
                //File name
                printf("File name: %s\n",buf);
                write(sock,buf,1024);
                // Size of file
                bytes_recieved=read(sock, forsbuf ,1024);
                forsbuf[bytes_recieved] = '\0';
                if(!strcmp(forsbuf,"Error"))
                    printf("No such file exists\n");
                else
                {
                    printf("Size of File: %s\n",forsbuf);
                    // File
                    read(sock,&recv_data_int,sizeof(int));
                    bytes_recieved = read(sock,buf2,1024);
                    buf2[bytes_recieved]='\0';
                    //printf("\n");
                    //      printf("--string is : %s recv_datda_int %d--",buf2, recv_data_int);
                    FILE *fd;
                    fd = fopen(copy,"w");
                    while(strcmp(buf2,"Over")!=0)
                    {
                        for(int i=0;i<recv_data_int;i++)
                            fprintf(fd,"%c",buf2[i]);
                        read(sock,&recv_data_int,sizeof(int));
                        bytes_recieved = read(sock,buf2,1024);
                        buf2[bytes_recieved]='\0';
                        //printf("string by parts id %s and %d\n",buf2, recv_data_int);
                    } 
                    fclose(fd);  
                    // MD5
                    bytes_recieved = read(sock, buf5, 1024);
                    buf5[bytes_recieved] = '\0';
                    printf("MD5 hash value is: %s\n", buf5);
                    //printf("\n");
                    // Date
                    bytes_recieved=read(sock, buf3 ,1024);
                    buf3[bytes_recieved] = '\0';
                    printf("Last Modified Date: %s\n",buf3);
                    //printf("\n");
                    //Time
                    bytes_recieved = read(sock, buf4 , 1024);
                    buf4[bytes_recieved] = '\0';
                    printf("Last Modified Time: %s\n",buf4);
                    //printf("\n");
                }
            }
            else
            {
                printf("Invalid Command.\nPlease view the README file for proper syntax of commands.\n");
            }
        }
//        if(cl == 1)
  //      {
    //        cl=0;
      //  }
    }   
            close(sock);
    return 0;
}
