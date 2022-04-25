typedef struct {
  int64_t after;
  uint16_t start;
  uint8_t limit;
} Query;

//解析query参数，放到Query q中去。如果不存在某项参数，则值默认为0。
int parse_query (Query *q, char *query) {
  int state = 0, c, p;
  int64_t x;
  if (!query) return 1;
  q->start = 0;
  q->after = 0;
  q->limit = 1;
  while (1) {
    c = *query;
    switch (state) {
    case 0:
      if (c == '\0') return 1;
      state++;
    case 1:
      p = c;  //记录开始位置
      state++;
      break;
    case 2:
      if (c != '=') return 0; //必须出现 “=”
      state++;
      break;
    case 3:
      if (digit (c)) x = (c - '0'); //获取到等号后面的数字
      else return 0;
      state++;
      break;
    case 4:
      switch (c) {
      case '&':
        state = 0;  //&用来隔断不同的查询变量，比如“l=2&s=3”
        query++;    //注意这里没有break，直接走到下一个case中。
      case '\0':  //对于最后一个查询参数，将遇到'\0'。
        state++;  //如果不是从上面的case进来的，则下一个state值是5，即完成解析，退出这个函数。
        switch (p) {
        case 'a':
          q->after = x;
          break;
        case 'l':
          q->limit = x;
          break;
        case 's':
          q->start = x;
          break;
        default:
          return 0;
        }
        break;
      default://如果还是一个数字，则说明是大于一位数的一个数字。
        if (digit (c)) x = x * 10 + (c - '0');
        else return 0;
        query++;
      }
      continue; //遇到“&”之后，开始下一个参数的解析。
    case 5:
      return 1;
    }
    query++;
  }
}
