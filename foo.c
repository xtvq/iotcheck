#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>


#define nom_list_crl_max 3

//status code
#define st_init 0
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
int[nom_list_crl_max] list_crl;

//local para 
//prefix lc_
int status;
int lc_hotwater;
int lc_temp;
int lc_enlign;

// functions net connect
int findnet();

// functions local
int open() {
	lc_hotwater=1;
	printf("hotwater open\n");
	}
int close() {
	lc_hotwater=0;
	printf("hotwater close\n");
	}
int wait(int t) {printf("wait t ms\n")};

// functions pipe
int get_incoming_msg(int childpid)
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
		
		printf("incoming msg : %i\n data: %i",incoming_msg,incoming_msg_data);
		return 0;
	} else
	{
		return 0;
	}
}


int main() {
	/*common local value*/
	nom_list_crl=0;
	temp=0;
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
	if(childpid == 0) for(;1;) {
	printf("loop !\n");
	
	get_incoming_msg(childpid);
	
	
	
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
			open();
			status=make_coffee2;
			wait(50);
			break;
		case make_coffee2 :
			wait(lc_temp);
			status=make_coffee3;
			wait(50);
			break;
		case make_coffee3 :
			close();
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
	
	wait();
	}

	 /*the mother loop*/
}
