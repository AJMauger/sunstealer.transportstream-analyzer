#define _GET_LE64(x) (((std::uint64_t)((std::uint8_t *)x)[0])<<56)+(((std::uint64_t)((std::uint8_t *)x)[1])<<48)+(((std::uint64_t)((std::uint8_t *)x)[2])<<40)+(((std::uint64_t)((std::uint8_t *)x)[3])<<32)+(((std::uint8_t *)x)[4]<<24)+(((std::uint8_t *)x)[5]<<16)+(((std::uint8_t *)x)[6]<<8)+((std::uint8_t *)x)[7]
#define _GET_LE48(x) (((std::uint64_t)((std::uint8_t *)x)[0])<<40)+(((std::uint64_t)((std::uint8_t *)x)[1])<<32)+(((std::uint8_t *)x)[2]<<24)+(((std::uint8_t *)x)[3]<<16)+(((std::uint8_t *)x)[4]<<8)+((std::uint8_t *)x)[5]
#define _GET_LE32(x) (((std::uint8_t *)x)[0]<<24)+(((std::uint8_t *)x)[1]<<16)+(((std::uint8_t *)x)[2]<<8)+((std::uint8_t *)x)[3]
#define _GET_LE24(x) (((std::uint8_t *)x)[0]<<16)+(((std::uint8_t *)x)[1]<<8)+((std::uint8_t *)x)[2]
#define _GET_LE16(x) (((std::uint8_t *)x)[0]<<8)+((std::uint8_t *)x)[1]
#define _GET_LE8(x)  ((std::uint8_t *)x)[0]

#define _GET_BE64(x) (((std::uint64_t)((std::uint8_t *)x)[7])<<56)+(((std::uint64_t)((std::uint8_t *)x)[6])<<48)+(((std::uint64_t)((std::uint8_t *)x)[5])<<40)+(((std::uint64_t)((std::uint8_t *)x)[4])<<32)+(((std::uint8_t *)x)[3]<<24)+(((std::uint8_t *)x)[2]<<16)+(((std::uint8_t *)x)[1]<<8)+((std::uint8_t *)x)[0]
#define _GET_BE48(x) (((std::uint64_t)((std::uint8_t *)x)[5])<<40)+(((std::uint64_t)((std::uint8_t *)x)[4])<<32)+(((std::uint8_t *)x)[3]<<24)+(((std::uint8_t *)x)[2]<<16)+(((std::uint8_t *)x)[1]<<8)+((std::uint8_t *)x)[0]
#define _GET_BE32(x) (((std::uint8_t *)x)[3]<<24)+(((std::uint8_t *)x)[2]<<16)+(((std::uint8_t *)x)[1]<<8)+((std::uint8_t *)x)[0]
#define _GET_BE24(x) (((std::uint8_t *)x)[2]<<16)+(((std::uint8_t *)x)[1]<<8)+((std::uint8_t *)x)[0]
#define _GET_BE16(x) (((std::uint8_t *)x)[1]<<8)+((std::uint8_t *)x)[0]
#define _GET_BE8(x)  ((std::uint8_t *)x)[0]

#define _GET_LEPID(x)  ((std::uint16_t)(((std::uint8_t *)x)[0]&0x1f)<<8)+((std::uint8_t *)x)[1]

#define _PUT_LE64(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[7];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[6];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[5];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[4];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[3];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[2];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++
#define _PUT_LE32(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[3];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[2];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++
#define _PUT_LE24(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[2];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++
#define _PUT_LE16(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++
#define _PUT_LE8(x, y)  *((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++

#define _PUT_BE64(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[2];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[3];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[4];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[5];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[6];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[7];((std::uint8_t *)x)++
#define _PUT_BE32(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[2];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[3];((std::uint8_t *)x)++
#define _PUT_BE24(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[2];((std::uint8_t *)x)++
#define _PUT_BE16(x, y) *((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++;*((std::uint8_t *)x)=((std::uint8_t *)&y)[1];((std::uint8_t *)x)++
#define _PUT_BE8(x, y)  *((std::uint8_t *)x)=((std::uint8_t *)&y)[0];((std::uint8_t *)x)++
