#include <stdio.h>        /* For printf() - printing to screen          */
#include <pcap/pcap.h>    /* The libpcap library - for capturing packets */
#include <arpa/inet.h>    /* For inet_ntop() - converts IP to text       */
#include <netinet/ip.h>   /* Gives us the 'struct ip' (IP header layout) */
#include <net/ethernet.h> /* Gives us 'struct ether_header' (Ethernet layout) */
#include<time.h>


void packet_handler(u_char *args,const struct pcap_pkthdr *header,const u_char *packet)
{
    
    struct ether_header *eth;   /* Pointer to the Ethernet header */
    struct ip            *ip;   /* Pointer to the IP header       */

    char src_ip[INET_ADDRSTRLEN]; /* Will hold source IP as text      */
    char dst_ip[INET_ADDRSTRLEN]; /* Will hold destination IP as text */


   
    if (header->len < sizeof(struct ether_header))
    {
        return;
    }


    eth = (struct ether_header *) packet;


    if (ntohs(eth->ether_type) != ETHERTYPE_IP)
    {
        return;
    }


    if (header->len < sizeof(struct ether_header) + sizeof(struct ip))
    {
        return;
    }


    ip = (struct ip *)(packet + sizeof(struct ether_header));


    inet_ntop(AF_INET, &(ip->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->ip_dst), dst_ip, INET_ADDRSTRLEN);

    // Network Type
    char *ether_typ=ntohs(eth->ether_type)==0x0800?"IPv4":ntohs(eth->ether_type)==0x0806?"ARP":"IPv6";

    printf("\n-----------------------------");
    printf("\n Packet Captured!");
    
    printf("\n Received Packet Length : %d", header->len);
    printf("\n Received At : %s", ctime((const time_t*) &header->ts.tv_sec));

    printf("\n Header Length : %d", ip->ip_hl*4);
    printf("\n Ethernet type : %s", ether_typ);
    printf("\n Ethernet header length : %zu", sizeof(struct ether_header));
    printf("\n");

    printf("\n Source IP      : %s", src_ip);
    printf("\n Destination IP : %s", dst_ip);
    printf("\n");
    
    printf("\n Source Mac Address : %02x:%02x:%02x:%02x:%02x:%02x", eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],eth->ether_shost[3],eth->ether_shost[4],eth->ether_shost[5]);
    printf("\n Destination Mac Address : %02x:%02x:%02x:%02x:%02x:%02x", eth->ether_dhost[0],eth->ether_dhost[1],eth->ether_dhost[2],eth->ether_dhost[3],eth->ether_dhost[4],eth->ether_dhost[5]);
    
    printf("\n-----------------------------");
}


int main()
{
    pcap_t *handle;

    char err[PCAP_ERRBUF_SIZE];
    char dev[]="eth0"; //Replace it with necessary interfaces like any or eth0 or wlo

    handle = pcap_open_live(dev, 65536, 1, 1000, err);
    if (handle == NULL)
    {
        printf("Error opening device: %s\n", err);
        printf("TIP: Try running with sudo, or check your interface name.\n");
        printf("     Run 'ip link' to list available network interfaces.\n");
        return 1; /* Return 1 to indicate an error occurred */
    }

    printf("Listening for packets on %s interface... Press Ctrl+C to stop.\n",dev);
    
    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_close(handle);

}
