/*
Sniffer.c V1.0.0

by Eric Liu

Based on libpcap 
Environment: Linux
To use this sniffer, please make sure you have libpcap installed in your system!!!
If you do not have libpcap:
	sudo yum install pcap
	or
	apt-get install pcap

Usage:
	gcc sniffer.c -o sniffer -lpcap
	sudo ./sniffer interface_name bridge_name (filter_file_name)
=======================================================
	cat filter
	--------------------------
	tcp and not src 123.125.114.144
	--------------------------
	
	Example:
		sudo ./sniffer wlan0 br0 filter
=======================================================

	Comment: 
	
	Sniffe all packets through interface and write them into net bridge br0.
	Syntax in filter is the same as tcpdump.
	You can use tcpdump or wireshark to check whether packets have been written to bridge or not.

*/
#include<pcap.h>
#include<linux/if_tun.h>
#include<string.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/ioctl.h>
#include<stdlib.h> 
#include<arpa/inet.h>
#include<linux/if.h>

#define MAX_FILTER_LENGTH 100


pcap_t *if_handle, *br_handle;
static char errbuf[PCAP_ERRBUF_SIZE];

#define Log  stdout

void usage(){
	printf("Usage: ./sniffer [interface] [bridge] [filter_expression_file_name](optional)\n");
	printf("[Interface]: Interface to be listened\n[bridge]: beidge to send packet to\n[filter_expression_file]: (optional)Expression to filter packe, default 'tcp and not [host_ip] and not src [host_ip]'(see tcpdump)\n");
}

int get_if_ip(char *dev, char *ip_str){
	pcap_if_t *alldevs;
	int status = pcap_findalldevs(&alldevs, errbuf);
	if(status != 0) {
		fprintf(Log, "%s\n", errbuf);
		return 0;
	}
	pcap_if_t *d;
	pcap_addr_t *a;
	for(d=alldevs; d!=NULL; d=d->next){
		if (strcmp(dev, d->name)==0){
			for (a=d->addresses; a!=NULL; a=a->next){
				if (a->addr->sa_family == AF_INET){
					strcpy( ip_str, inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr) );
					return 1;
				}
			}
		}
	}

	return 0;

}


void packet_forward(u_char *args, const struct pcap_pkthdr *header, u_char *packet){
	//Set dest MAC as Broadcast Address
	int i;
	for (i=0;i<6;i++){ 
		*(packet+i) = 0xff;
	}
/*
	const u_char *ptr = packet;
	fprintf(Log, "For packet:\n");
	for (i=0;i<header->len;i++){
		if (i % 20 == 0) fprintf(Log, "\n");
		fprintf(Log, "%02x ",*ptr);
		ptr++;
	}	
	fprintf(Log, "\n\n");
*/
	if (pcap_sendpacket(br_handle, packet, header->len) != 0){
		fprintf(Log, "[ERROR]Failed to sent out packet!\n");
		exit(5);
	}

	fprintf(Log, "Sent packet to bridge!\n");
	
}

int main(int argc, char *argv[]){
	if ( argc != 3 && argc!=4){
		fprintf(Log, "[ERROR]Useage Error! Print Usage\n");
		usage();
		return 1;
	}

	fprintf(Log, "Start sniffing...\n");
	
	char br_dev[IFNAMSIZ], if_dev[IFNAMSIZ]; //device name and error buff
	//Filter program
	struct bpf_program fp;
	char filter_exp[100];
	if (argc == 4){
		fprintf(Log, "Reading filter..\n");
		FILE *filter_file = fopen(argv[3],"r");

		if (!filter_file){
			fprintf(Log, "Can't open filter file %s\n", argv[3]);
			return 7;
		}
		fgets(filter_exp, 100, filter_file);
		fprintf(Log, "Read filter [%s]\n", filter_exp);
	}

	strncpy(if_dev, argv[1], IFNAMSIZ);
	strncpy(br_dev, argv[2], IFNAMSIZ);

	fprintf(Log, "Opening interface %s...\n", if_dev);
	if ((if_handle = pcap_open_live(if_dev,BUFSIZ, 1, 0, errbuf)) == NULL){
		fprintf(Log, "[ERROR]Can't open interface %s\n",if_dev);
		return 2;
	}
	fprintf(Log, "Interface %s opened!\n", if_dev);

	fprintf(Log, "Opening bridge %s...\n", br_dev);
	if ((br_handle = pcap_open_live(br_dev,BUFSIZ, 1, 0, errbuf)) == NULL ){
		fprintf(Log, "[ERROR]Can't open bridge %s\n",br_dev);
		return 2;
	}
	fprintf(Log ,"Bridge %s opened!\n", br_dev);

	//Create a filter for pcap
	fprintf(Log, "Creating filter\n");
	if (argc == 3){
		char ip_str[20];
		fprintf(Log, "Getting IP address of interfac...\n");
		if (!get_if_ip(if_dev, ip_str)){
			fprintf(Log,"Failed to get IP address!\n");
			return  3;
		}
		fprintf(Log, "Got IP address: %s\n",ip_str);
		strcpy(filter_exp, "tcp and not dst ");
		strcat(filter_exp, ip_str);
		strcat(filter_exp," and not src ");
		strcat(filter_exp, ip_str);
	}
	
	fprintf(Log, "Comiling filter:%s\n",filter_exp);
	if (pcap_compile(if_handle, &fp, filter_exp, 0, 0) == -1){
		fprintf(Log, "[ERROR]Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(if_handle));
		return 3;
	}

	fprintf(Log, "Successfully compiled filter %s\n",filter_exp);

	fprintf(Log, "Setting filter...\n");

	if (pcap_setfilter(if_handle, &fp)==-1){
		fprintf(Log, "[ERROR]Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(if_handle));
		return 4;
	}
	
	
	fprintf(Log, "Successfully set filter: %s\n", filter_exp);
	
	//Loop sniffing packets
	pcap_loop(if_handle, -1, packet_forward, NULL);
	pcap_close(br_handle);
	pcap_close(if_handle);
	fprintf(Log, "Finish sniffing!\n");
	return 0;

}
