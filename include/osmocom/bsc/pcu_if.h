#ifndef _PCU_IF_H
#define _PCU_IF_H

#include <osmocom/gsm/l1sap.h>

extern int pcu_direct;

#define PCUIF_HDR_SIZE (sizeof(struct gsm_pcu_if) - sizeof(((struct gsm_pcu_if *)0)->u))

struct pcu_sock_state {
	struct gsm_network *net;	/* backpointer to GSM network */
	struct osmo_fd listen_bfd;	/* fd for listen socket */
	struct osmo_fd conn_bfd;	/* fd for connection to lcr */
	struct llist_head upqueue;	/* queue for sending messages */
};

/* Check if BTS has a PCU connection */
bool pcu_connected(const struct gsm_network *net);

/* PCU relevant information has changed; Inform PCU (if connected) */
void pcu_info_update(struct gsm_bts *bts);

/* Forward rach indication to PCU */
int pcu_tx_rach_ind(struct gsm_bts *bts, int16_t qta, uint16_t ra, uint32_t fn,
	uint8_t is_11bit, enum ph_burst_type burst_type);

/* Confirm the sending of an AGCH or PCH MAC block to the pcu */
int pcu_tx_data_cnf(struct gsm_bts *bts, uint32_t msg_id, uint8_t sapi);

/* Open connection to PCU */
int pcu_sock_init(struct gsm_network *net);

/* Close connection to PCU */
void pcu_sock_exit(struct gsm_network *net);

#endif /* _PCU_IF_H */
