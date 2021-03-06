#ifndef _PROXY_I_H
#define _PROXY_I_H
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <arch.h>
#include <netmsg.h>
#include <proxy.h>
#include <sys/epoll.h>

/**
 * socket API porting layer for BSD socket and mTCP socket
 */
#define _pcn_socket(__d,__t,__p)               socket(__d,__t,__p)
#define _pcn_bind(__s,__a,__l)                 bind(__s,__a,__l)
#define _pcn_connect(__s,__a,__l)              connect(__s,__a,__l)
#define _pcn_listen(__s,__b)                   listen(__s,__b)
#define _pcn_setsockopt(__s,__l,__n,__v,__e)   setsockopt(__s,__l,__n,__v,__e)
#define _pcn_getsockopt(__s,__l,__n,__v,__e)   getsockopt(__s,__l,__n,__v,__e)
#define _pcn_sendmsg(__s,__m,__f)              sendmsg(__s,__m,__f)
#define _pcn_senddata(__s,__b,__l,__f)         send(__s,__b,__l,__f)
#define _pcn_recv(__s,__b,__l,__f)	       recv(__s,__b,__l,__f)
#define _pcn_epoll_create(__n)		       epoll_create(__n)
#define _pcn_epoll_wait(__s,__e,__m,__t)       epoll_wait(__s,__e,__m,__t)
#define _pcn_epoll_ctl(__s,__o,__f,__e)	       epoll_ctl(__s,__o,__f,__e)
#define _pcn_shutdown(__s,__f)                 shutdown(__s,__f)

static inline
int _pcn_close(struct pcn_netctx_t *netctx, int fd)
{
	int rc = 0;
	int zombie;

	zombie = smp_swap(&netctx->zombie_fd, fd);
	if (zombie) {
		rc = close(zombie);
	}
	return rc;
}

/**
 * internal functions
 */
static int   _pcn_init_netctx(struct pcn_proxy_t *proxy, int netctx_id,
			      struct pcn_netctx_t *netctx);
static void  _pcn_deinit_netctx(struct pcn_netctx_t *netctx);
static void *_pcn_netctx_worker(void *_p);
static int   _pcn_netctx_process(struct pcn_netctx_t *netctx);
static int   _pcn_netctx_process_channels(struct pcn_netctx_t *netctx);
static int   _pcn_netctx_process_sockets(struct pcn_netctx_t *netctx);
static int   _pcn_channel_process_netmsg(struct pcn_netctx_t *netctx,
					 int remote_id,
					 struct ring_buffer_req_t *in_msg);
static int   _pcn_secure_scratchpad(struct pcn_netctx_t *netctx, int size);
static int   _pcn_channel_process_t_socket(struct pcn_netctx_t *netctx,
					   int remote_id,
					   struct netmsg_t_socket_t *t_socket);
static int   _pcn_channel_process_t_bind(struct pcn_netctx_t *netctx,
					 int remote_id,
					 struct netmsg_t_bind_t *t_bind);
static int   _pcn_channel_process_t_connect(struct pcn_netctx_t *netctx,
					    int remote_id,
					    struct netmsg_t_connect_t *t_connect);
static int   _pcn_channel_process_t_listen(struct pcn_netctx_t *netctx,
					   int remote_id,
					   struct netmsg_t_listen_t *t_listen);
static int   _pcn_channel_process_t_setsockopt(struct pcn_netctx_t *netctx,
					       int remote_id,
					       struct netmsg_t_setsockopt_t *t_setsockopt);
static int   _pcn_channel_process_t_getsockopt(struct pcn_netctx_t *netctx,
					       int remote_id,
					       struct netmsg_t_getsockopt_t *t_getsockopt);
static int   _pcn_channel_process_t_sendmsg(struct pcn_netctx_t *netctx,
					    int remote_id,
					    struct netmsg_t_sendmsg_t *t_sendmsg);
static int   _pcn_channel_process_t_senddata(struct pcn_netctx_t *netctx,
					    int remote_id,
					    struct netmsg_t_senddata_t *t_senddata);
static int   _pcn_channel_process_t_shutdown(struct pcn_netctx_t *netctx,
					     int remote_id,
					     struct netmsg_t_shutdown_t *t_shutdown);
static int   _pcn_channel_process_t_close(struct pcn_netctx_t *netctx,
					  int remote_id,
					  struct netmsg_t_close_t *t_close);

/**
 * Util Functions
 */
int proxy_core_affinitize(int cpu);
#endif /* _PROXY_I_H */
