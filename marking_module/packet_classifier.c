/* Code referred from http://stackoverflow.com/questions/29553990/print-tcp-packet-data and
 http://www.linuxjournal.com/article/7184 */
#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

#include <linux/ip.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>

static struct nf_hook_ops netfilter_ops;

unsigned short cksum(struct iphdr *ip, int len) {
    int sum = 0;  /* assume 32 bit int, 16 bit short */

    while(len > 1){
        sum += *((unsigned short*) ip);
        (unsigned short *) ip++;

        if(sum & 0x80000000)   /* if high order bit set, fold */
            sum = (sum & 0xFFFF) + (sum >> 16);
        len -= 2;
    }

    if(len)       /* take care of left over byte */
        sum += (unsigned short) *(unsigned char *)ip;

    while(sum>>16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ~sum;
}
         
/* taken from TCP/IP Illustrated Vol. 2(1995) by Gary R. Wright and W. Richard Stevens. Page 236 */

static unsigned int main_hook(const struct nf_hook_ops *ops,
                              struct sk_buff *sock_buff,
                              const struct net_device *in,
                              const struct net_device *out,
                              int (*okfn)(struct sk_buff*)) {
    
    static struct iphdr *iph;          /* IPv4 header */
    static struct udphdr *udph;        /* UDP header */
    static u16 sport, dport, udp_len;   /* Source and destination ports */
    static u32 saddr, daddr;           /* Source and destination addresses */
    static u8 *rtp_pointer = 0;
    static u8 *mpeg_pointer = 0;
    static u8 cc = 0;
    static u8 *rtp_ehl = 0;
    static u16 ehl_size = 0;
    
    /* Network packet is empty, seems like some problem occurred. Skip it */
    if(!sock_buff) {
        return NF_ACCEPT;
    }
    
    //sock_buff->ip_summed = CHECKSUM_NONE; //Stop checksum offloading

    /* Get IP Header */
    iph = ip_hdr(sock_buff);

    /* Skip if its ipv6 */
    if(!iph)
	    return NF_ACCEPT;

    /* RTP payload is sent over UDP packet, skip other packets */
    if (iph->protocol != IPPROTO_UDP)
	    return NF_ACCEPT;

    /* Skip ICMP packets 
    if(iph->protocol == IPPROTO_ICMP)
	    return NF_ACCEPT;
    */
    /* Get UDP Header */
    udph = udp_hdr(sock_buff);
    
    /* Convert network endianness to host endiannes */
    sport = ntohs(udph->source);
    dport = ntohs(udph->dest);
    udp_len = ntohs(udph->len);
    saddr = ntohl(iph->saddr);
    daddr = ntohl(iph->daddr);

    /* Skip if Source and Destination port of UDP is < 1024 */
    if(sport < 1024 || dport != 5004)
	    return NF_ACCEPT;
    //printk(KERN_ALERT "port number %d\n", dport);
    /* Check if length of  UDP payload is atleast RTP header */
    if(udp_len  > 20) {

	    rtp_pointer = ((u8 *) udph + 8);
	    cc = rtp_pointer[0] & 0x0f;

	    if (cc > 0 && udp_len < (20 + (4 * cc))) {
            return NF_ACCEPT;
        }
    }
    //printk (KERN_ALERT "udp length %u cc %u\n", udp_len, cc);
            

    /* Check whether Extension header is present or not */
    if((rtp_pointer[0] & 0x10) == 0x10) {
        printk(KERN_ALERT "Extension header is present\n");
	    rtp_ehl = rtp_pointer + (14 + (4*cc));
	    ehl_size = (rtp_ehl[1] | rtp_ehl[0] << 8);
    }
    //printk (KERN_ALERT "ehl check 0x%x ehl size %u\n", (rtp_pointer[0] & 0x10), ehl_size);
    /* Check if RTP version is 2 */
    if((rtp_pointer[0] & 0xC0) != 0x80){
        return NF_ACCEPT;
    }
    //printk(KERN_ALERT "rtp type 0x%x\n", ((rtp_pointer[0] & 0xC0)));
        
    /* Check if RTP payload is MPEG-1 or MPEG-2 */
    
    //printk(KERN_ALERT "rtp_pointer[1] %x\n", rtp_pointer[1] & 0x7E); 
    
    if((rtp_pointer[1] & 0x7E) == 0x20) {
	    //Classification Logic
	    //printk(KERN_ALERT "MPEG-1\n");
            //mpeg_pointer = rtp_pointer + (12 + (4 * cc)) + (4 * ehl_size);
            mpeg_pointer = rtp_pointer + (12 + (4 * cc));
	    mpeg_pointer = mpeg_pointer + 3;
	    //Check whether the packet is I packet or not
	    if((mpeg_pointer[0] & 0x07) < 3) {
                    //printk (KERN_ALERT "tos field change\n");
                    //printk (KERN_ALERT "Mpeg byte 3 0x%x\n", mpeg_pointer[0]);
                    //printk (KERN_ALERT "Seq No. %u\n", ntohs(*(unsigned short *) &rtp_pointer[2]));
		    iph->tos = 184;
                    //printk (KERN_ALERT "TOS byte 0x%x\n", *((u8 *)((u8 *)iph + 1)));
		    iph->check = 0;
                    iph->check = ip_fast_csum((u8 *)iph, iph->ihl);
                    printk (KERN_ALERT "checksum %x\n", iph->check);
	    }
    }

    return NF_ACCEPT;
}

/* Register Handler */
static int __init f_init_module(void)
{
    int err_chk;
    netfilter_ops.hook             =       (nf_hookfn *)main_hook;
    netfilter_ops.pf               =       PF_INET;
    netfilter_ops.hooknum          =       NF_INET_POST_ROUTING;
    netfilter_ops.priority         =       NF_IP_PRI_FIRST;
    err_chk = nf_register_hook(&netfilter_ops);
    if(err_chk < 0)
        pr_err("Error in netfilter hook\n");
    return 0;
}
/* CLeanup */
static void __exit f_cleanup_module(void) {
    nf_unregister_hook(&netfilter_ops);
    pr_info("Unloaded packet classifier module\n");
}

module_init(f_init_module);
module_exit(f_cleanup_module);
