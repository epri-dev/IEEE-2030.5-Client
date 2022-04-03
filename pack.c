// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#define PACK16(m,d) *(m) = (char)((d)>>8); *(m+1) = (char)(d) //将16bit数据d放到以m开头的连续地址上，大端模式。
#define PACK32(m,d) PACK16(m,d>>16); PACK16(m+2,d)  //将32bit数据放到以m开头的连续地址上，大端模式。
#define PACK64(m,d) PACK32(m,d>>32); PACK32(m+4,d)
#define UNPACK16(m) ((((unsigned char)*(m)) << 8)|((unsigned char)*(m+1)))  //将以大端模式存放的两个字节合并成一个16bit数据
#define UNPACK32(m) ((UNPACK16(m) << 16)|UNPACK16(m+2))
#define UNPACK16_LE(m) ((((unsigned char)*(m+1)) << 8)|((unsigned char)*(m))) //将以小端模式存放的两个字节合并成一个16bit数据。
#define UNPACK32_LE(m) ((UNPACK16_LE(m+2) << 16)|UNPACK16_LE(m))  //同上，将以小端模式存放的数据合并成32bit数据。
