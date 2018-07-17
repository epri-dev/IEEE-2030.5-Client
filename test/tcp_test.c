#include "../pack.c"
#include "../util.c"
#include "../list.c"
#include "../queue.c"
#include "../platform.c"
// case TCP_PORT:
//      TCP_CONNECT
//  TCP_ACCEPT
//  TCP_CLOSED
//  TCP_TIMEOUT

TcpPort *client, *server;

int established (void *conn, int event) {
  char buffer[1];
  net_read (conn, buffer, 1);
  switch (event) {
  case TCP_ACCEPT:
    printf ("  TCP_ACCEPT: connection accepted\n"); return 1;
  case TCP_CONNECT:
    printf ("  TCP_CONNECT: client connected\n"); return 2;
  } return 0;
}

int closed (void *conn, int event) {
  switch (event) {
  case TCP_CLOSED:
    if (conn == client) {
      printf ("  TCP_CLOSED: client side\n"); return 1;
    }
    if (conn == server) {
      printf ("  TCP_CLOSED: server side\n"); return 2;
    }
  } return 0;
}
      
int main () {
  char zero[16] = {0};
  int state = 0, event, cond;
  Acceptor *a; void *any;
  Address addr1, addr2;
  address_copy (&addr1, ipv6_address (zero, 12345));
  address_copy (&addr2, ipv6_address (zero, 54321));
  printf ("TcpPort test\n");
  platform_init ();
  a = net_listen (&addr1);
  client = new_tcp_port ();
  server = new_tcp_port ();
  net_timeout (2);
  while (1) {
    switch (state) {
    case 0: cond = 0;
      printf ("test 0 - client connect\n");
      net_connect (client, &addr1);
      net_accept (server, a);
      state++; break;
    case 1:
      cond |= established (any, event);
      if (cond == 3) { cond = 0;
	printf ("\ntest 1 - client close\n");
	net_close (client); state++;
      } break;
    case 2:
      cond |= closed (any, event);
      if (cond == 3) { cond = 0;
	printf ("\ntest 2 - server accept\n");
	net_accept (server, a);
	net_connect (client, &addr1);
	state++;
      } break;
    case 3:
      cond |= established (any, event);
      if (cond == 3) { cond = 0;
	printf ("\ntest 3 - server close\n");
	net_close (server); state++;
      } break;
    case 4:
      cond |= closed (any, event);
      if (cond == 3) { cond = 0;
	printf ("\ntest 4 - client connect timeout\n");
	net_connect (client, &addr1); state++;
      } break;
    case 5:
      switch (event) {
      case TCP_TIMEOUT:
	printf ("  TCP_TIMEOUT: connect timed out\n");
	net_close (client);
	printf ("\ntest 5 - client connect blocked\n");
	net_connect (client, &addr2);
	state++; break;
      } break;
    case 6:
      cond |= closed (any, event);
      if (cond == 1) {
      }
    }
    event = event_poll (&any, -1);
  }
}
