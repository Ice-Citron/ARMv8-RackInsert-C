// Symbol table:
#define MAXSYMBOLLEN 64
#define MAXSYMBOLS (1 << 21)
//i should be replace this with # of memory slots but im lazy
typedef char symbol[MAXSYMBOLLEN];

typedef struct {
	symbol s;
	unsigned v;
} symval;