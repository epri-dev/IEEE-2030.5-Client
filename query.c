typedef struct {
  int64_t after;
  uint16_t start;
  uint8_t limit;
} Query;

int parse_query (Query *q, char *query) {
  int state = 0, c, p; int64_t x;
  if (!query) return 1;
  q->start = 0; q->after = 0; q->limit = 1;
  while (1) { c = *query;
    switch (state) {
    case 0: if (c == '\0') return 1; state++;
    case 1: p = c; state++; break;
    case 2: if (c != '=') return 0; state++; break;
    case 3: if (digit (c)) x = (c - '0'); else return 0; state++; break;
    case 4: switch (c) {
      case '&': state = 0; query++;
      case '\0': state++; switch (p) {
	case 'a': q->after = x; break;
	case 'l': q->limit = x; break;
	case 's': q->start = x; break;
	default: return 0;
	} break;
      default: if (digit (c)) x = x * 10 + (c - '0');
	else return 0; query++; 
      } continue;
    case 5: return 1;
    }
    query++;
  }
}
