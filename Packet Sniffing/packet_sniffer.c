/* ============================================================ */
/*  NETWORK PACKET SNIFFER — Complete Beginner-Friendly Version */
/*  Compile: gcc packet_sniffer.c -o sniffer -lpcap             */
/*  Run:     sudo ./sniffer                                      */
/* ============================================================ */

#include <stdio.h>           /* printf()                  */
#include <pcap/pcap.h>       /* libpcap functions         */
#include <arpa/inet.h>       /* inet_ntop()               */
#include <netinet/ip.h>      /* struct ip                 */
#include <net/ethernet.h>    /* struct ether_header       */

/* ---- CALLBACK FUNCTION: called for every packet ---- */
void packet_handler(u_char *args,
                    const struct pcap_pkthdr *header,
                    const u_char *packet)
{
    struct ether_header *eth;         /* Ethernet header pointer */
    struct ip           *ip;          /* IP header pointer       */
    char src_ip[INET_ADDRSTRLEN];     /* Source IP as text       */
    char dst_ip[INET_ADDRSTRLEN];     /* Destination IP as text  */

    /* SAFETY CHECK 1: packet must be big enough for Ethernet */
    if (header->len < sizeof(struct ether_header)) return;

    /* STEP 1: Point eth to the start of packet */
    eth = (struct ether_header *) packet;

    /* STEP 2: Skip non-IP packets (ARP, IPv6, etc.) */
    if (ntohs(eth->ether_type) != ETHERTYPE_IP) return;

    /* SAFETY CHECK 2: packet must hold IP header too */
    if (header->len < sizeof(struct ether_header) +
                      sizeof(struct ip)) return;

    /* STEP 3: Point ip to right after the Ethernet header */
    ip = (struct ip *)(packet + sizeof(struct ether_header));

    /* STEP 4: Convert binary IPs to readable text strings */
    inet_ntop(AF_INET, &(ip->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->ip_dst), dst_ip, INET_ADDRSTRLEN);

    /* STEP 5: Print the results */
    printf("\n------------------------------");
    printf("\nSource IP      : %s", src_ip);
    printf("\nDestination IP : %s", dst_ip);
    printf("\n------------------------------");
}


int main() {
    pcap_t *handle;             
    char    err[PCAP_ERRBUF_SIZE]; /* Error buffer  */

    handle = pcap_open_live("eth0", 65536, 1, 1000, err);
    if (handle == NULL) {
        printf("Error: %s\n", err); return 1;
    }
    printf("Listening... Press Ctrl+C to stop.\n");
    pcap_loop(handle, 0, packet_handler, NULL);
    pcap_close(handle);
    return 0;
}
