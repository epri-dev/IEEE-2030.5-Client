#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

char *hex4 (int *x, char *data) { int c = *data;
  if (in_range (c, '0', '9')) *x = c - '0';
  else if (in_range (c, 'a', 'f')) *x = c - 'a' + 10;
  else if (in_range (c, 'A', 'F')) *x = c - 'A' + 10;
  else return NULL;
  return data + 1;
}

char *hex16 (char *h, char *data) {
  int n = 0, x = 0, y;
  while (n < 4) {
    if (hex4 (&y, data)) x = (x << 4) + y;
    else if (in_range (n, 1, 4)) goto out;
    else return NULL;
    n++; data++;
  }
 out:
  h[0] = x >> 8; h[1] = x;
  return data;
}

int ipv6_input (char c) {
  switch (c) {
  case ':': return 0;
  case ']': return 1;
  default: return 2;
  }
}

// input:
//  :  ]  other
const char ipv6_state[] =
  { 1, 7, 5,   // 0 [
    2, 7, 7,   // 1 [:
    7, 8, 3,   // 2 [::
    4, 8, 7,   // 3 [::x
    7, 7, 3,   // 4 [::x:
    6, 9, 7,   // 5 [x
    2, 7, 5};  // 6 [x:

char *parse_ipv6 (char *addr, char *data) {
  int state = 0, c, n = 0, m = 0, space; char *gap;
  ok (*data++ == '[');
  while ((c = *data)){
    state = ipv6_state[state*3 + ipv6_input (c)];
    switch (state) {
    case 2: gap = addr; break;
    case 3: m++; case 5: n++;
      ok (data = hex16 (addr, data));
      addr += 2; continue;
    case 7: return NULL; // input error
    case 8: // accept with gap
      n <<= 1; m <<= 1; space = 16 - n;
      memmove (gap+space, gap, m);
      memset (gap, 0, space);
      return data+1;
    case 9: return n == 8? data+1 : NULL;
    case 4: if (n == 6) return NULL; break;
    case 6: if (n == 8) return NULL;
    } data++;
  } return NULL;
}

char *parse_ipv4 (uint32_t *addr, char *data) {
  int x = 0, y, n = 0;
  do {
    ok ((data = number (&y, data)) && y <= 255);
    x = (x << 8) | y;
    if (++n == 4) { *addr = x; return data; }
  } while (*data++ == '.');
  return NULL;
}

char *parse_address (Address *addr, int port, char *data) {
  if (*data == '[') { char buffer[16];
    ok (data = parse_ipv6 (buffer, data));
    ipv6_address (addr, buffer, port);
  } else { uint32_t ipv4;
    ok (data = parse_ipv4 (&ipv4, data));
    ipv4_address (addr, ipv4, port);
  } return data;
}

int write_ipv4 (char *buffer, uint32_t addr) {
  uint8_t a, b, c, d;
  d = addr; addr >>= 8; c = addr; addr >>= 8;
  b = addr; addr >>= 8; a = addr;
  return sprintf (buffer, "%d.%d.%d.%d", a, b, c, d);
}

// write 128 bit host number to buffer in IPv6 format
int write_ipv6 (char *buffer, unsigned char *host) {
  int i, n = 0, zero = 0;
  *buffer++ = '[';
  for (i = 0; i < 16; i += 2) {
    int x = (host[i] << 8) | host[i+1];
    if (zero >= 0) {
      if (x == 0) { zero = 1; continue; }
      if (zero == 1) { zero = -1; buffer[n++] = ':'; }
    }
    if (n) buffer[n++] = ':';
    n += sprintf (buffer+n, "%x", x);
  }
  if (!n) n = sprintf (buffer, "::");
  return 1 + n + sprintf (buffer+n, "]");
}

int write_address (char *buffer, Address *addr) {
  switch (address_type (addr)) {
  case ADDR_IPv4: return write_ipv4 (buffer, address_ipv4 (addr));
  case ADDR_IPv6: return write_ipv6 (buffer, address_ipv6 (addr));
  } return 0; 
}

int write_address_port (char *buffer, Address *addr) {
  int n = write_address (buffer, addr);
  return n + sprintf (buffer+n, ":%d", address_port (addr));
}
