struct Cn16Char                  // 汉字字模数据结构 
{
	unsigned char  Index[4];            // 汉字内码索引,一个汉字占两个字节	
	unsigned char  Msk[32];             // 点阵码数据(16*16有32个数据) 
};
extern const unsigned char code F6x8[][6];
extern const unsigned char code F8X16[];
extern struct Cn16Char code CN16CHAR[];