// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#define PACK16(m,d) *(m) = (char)((d)>>8); *(m+1) = (char)(d)
#define PACK32(m,d) PACK16(m,d>>16); PACK16(m+2,d)
#define PACK64(m,d) PACK32(m,d>>32); PACK32(m+4,d)
#define UNPACK16(m) ((((unsigned char)*(m)) << 8)|((unsigned char)*(m+1)))
#define UNPACK32(m) ((UNPACK16(m) << 16)|UNPACK16(m+2))
#define UNPACK16_LE(m) ((((unsigned char)*(m+1)) << 8)|((unsigned char)*(m)))
#define UNPACK32_LE(m) ((UNPACK16_LE(m+2) << 16)|UNPACK16_LE(m))
