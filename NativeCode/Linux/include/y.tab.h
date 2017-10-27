typedef union
{
int  ival;
char *sval;
char bval;
char cval;
} GPIB_YYSTYPE;
#define	T_CONFIG	257
#define	T_DEVICE	258
#define	T_NAME	259
#define	T_BASE	260
#define	T_IRQ	261
#define	T_DMA	262
#define	T_DMABUF	263
#define	T_PAD	264
#define	T_SAD	265
#define	T_TIMO	266
#define	T_EOSBYTE	267
#define	T_DEBUG	268
#define	T_REOS	269
#define	T_XEOS	270
#define	T_BIN	271
#define	T_ERRLOG	272
#define	T_INIT_S	273
#define	T_DCL	274
#define	T_IFC	275
#define	T_MASTER	276
#define	T_LLO	277
#define	T_EXCL	278
#define	T_INIT_F	279
#define	T_NETWORK	280
#define	T_SERVER	281
#define	T_AUTOPOLL	282
#define	T_NUMBER	283
#define	T_STRING	284
#define	T_BOOL	285
#define	T_TIVAL	286


extern GPIB_YYSTYPE gpib_yylval;
