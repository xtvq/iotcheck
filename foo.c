#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>


#define nom_list_crl_max 3

//status code
#define st_init       101
#define find_net1     102
#define find_net2     103

#define wait_enlign   104
#define wait_offlign  105

#define make_coffee1  106
#define make_coffee2  107
#define make_coffee3  108

#define set_para1     109
#define set_para2     110


#define msg_veri      201
#define msg_para      202

#define max_nom_loop 10
#define macro_wait if (lc_enlign=0)\
				{\
					status=wait_enlign;\
					wait(50);\
				} else \
				{\
					status=wait_offlign;\
					wait(50);\
				}\
//pipes to test
//should not use this directly
int pipe_msg[2];
int pipe_msg_data[2];


int ID_net;
int nom_list_crl;
int list_crl[5];

//local para 
//prefix lc_
int status;

int lc_hotwater;
int lc_temp;
int lc_enlign;

// functions net connect
int findnet();

// functions local
int lc_open() {
	lc_hotwater=1;
	printf("hotwater open\n");
	}
int lc_close() {
	lc_hotwater=0;
	printf("hotwater close\n");
	}
int wait(int t) {
	int i,j;
	int w;
	i=0;j=0;
	for(;i<100000;i++)
		for (;j<100000;j++)
		w=i*j;
	printf("wait %i ms\n",t);};

// functions pipe
int get_incoming_msg(int childpid,int incoming_msg,int incoming_msg_data)
{
	int nbytes;
	
	int readbuffer;
	if (childpid==0)
	{
		//child process
		nbytes = read(pipe_msg[0], readbuffer, sizeof(readbuffer));
        incoming_msg=readbuffer;
		nbytes = read(pipe_msg_data[0], readbuffer, sizeof(readbuffer));
        incoming_msg_data=readbuffer;
		
		printf("incoming msg : %i data: %i\n ",incoming_msg,incoming_msg_data);
		return 0;
	} else
	{
		return 0;
	}
}
// functions pipe
int sent_incoming_msg(int childpid,int incoming_msg,int incoming_msg_data)
{
	if (childpid==0)
	{
		
		
		return 0;
	} else
	{
		write(pipe_msg[1], incoming_msg, (strlen(incoming_msg)+1));
		write(pipe_msg_data[1], incoming_msg_data, (strlen(incoming_msg_data)+1));
		return 0;
	}
}

int main() {
	/*common local value*/
	nom_list_crl=0;
	lc_temp=0;
	status=st_init;
	
	
	
	pid_t   childpid;

    pipe(pipe_msg);
	pipe(pipe_msg_data);
        
	if( (childpid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}

	if(childpid == 0)
	{
		/* Child process closes up input side of pipe */
		close(pipe_msg[0]);
		close(pipe_msg_data[0]);
	
		printf("v1 finish booting !\n");
	
	}
	else
	{
		/* Parent process closes up output side of pipe */
		close(pipe_msg[1]);
		close(pipe_msg_data[1]);
	
		printf("cafe finish booting !\n");
	
	}
	
	/*the children loop*/
	int nom_loop =0;
	if(childpid == 0) for(;nom_loop<max_nom_loop;nom_loop++) 
	{
		printf("loop cafe! childpid = %i\n",childpid);
		
		
		int incoming_msg=0;
		int incoming_msg_data=0;
		get_incoming_msg(childpid,incoming_msg,incoming_msg_data);
		
		
		
		switch (status) {
			case st_init :
				
				status=find_net1;
				break;
			case find_net1 :
				if (incoming_msg&&(incoming_msg==msg_veri))
				{
					printf("find net !\n");
					nom_list_crl++;
					list_crl[nom_list_crl]=incoming_msg_data;
					status=find_net2;
				} else 
				{
					status=find_net1;
					wait(50);
				}
				break;
			case find_net2 :
				if (incoming_msg&&(incoming_msg==msg_para))
				{
					printf("set para !\n");
					
					lc_temp=incoming_msg_data;
					status =wait_enlign;
				} else 
				{
					status=find_net2;
					wait(50);
				}
				break;
			case wait_enlign :
				if (incoming_msg&&(incoming_msg==msg_para))
				{
					status =set_para1;
				} else 
				{
					status=wait_enlign;
					wait(50);
				}
				break;
			case wait_offlign :
			
			
			case make_coffee1 :
				lc_open();
				status=make_coffee2;
				wait(50);
				break;
			case make_coffee2 :
				wait(lc_temp);
				status=make_coffee3;
				wait(50);
				break;
			case make_coffee3 :
				lc_close();
				status=make_coffee2;
				wait(50);
				break;
			case set_para1 :
				if (incoming_msg&&incoming_msg==msg_para)
				{
					printf("set para !\n");
					
					lc_temp=incoming_msg_data;
					macro_wait
				} else 
				{
					status = set_para1;
					wait(50);
				}
				break;
		}
		
		wait(50);
	} //end child loop
	/*the mother loop*/
	else for(;nom_loop<max_nom_loop;nom_loop++)
	{
		printf("loop v1! childpid = %i\n",childpid);
		int incoming_msg=msg_veri;
		int incoming_msg_data=15;
		
		sent_incoming_msg(childpid,incoming_msg,incoming_msg_data);
		wait(50);
		incoming_msg=msg_para;
		sent_incoming_msg(childpid,incoming_msg,incoming_msg_data);
		
	
		
		wait(50);
	} //end parent loop
}//end of main
