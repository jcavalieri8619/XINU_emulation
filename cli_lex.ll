%{
#include <mem.h>
#include <kernel.h>
#include <proc.h>

#include <cli_symbols.h>
%}

%%

[sS][hH][oO][wW]		return(SHOW);

[sS][uU][sS][pP][eE][nN][dD]	return(SUSPEND);

[rR][eE][sS][uU][mM][eE]	return(RESUME);

[kK][iI][lL][lL]		return(KILL);

[cC][rR][eE][aA][tT][eE]	return(CREATE);

[pP][rR][oO][cC]		return(PROC);

[rR][dD][yY]			return(RDY);

[sS][lL][pP]			return(SLP);

[wW][tT][rR]			return(WTR);

[rR][cC][vV]			return(RCV);

[s][e][m][a][p][h][o][r][e]     return(SEMAPHORE);

[lL][oO][oO][pP]		return(LOOP);

[s][i][g][n][a][l]              return(SEMSIGNL);

[eE][xX][iI][tT]		return(EXIT);

\".+\"				return(STR);

-?[0-9][0-9]*                   return(INTEG);

[A-Za-z][0-9A-Za-z]*:		return(LABEL);

;.*\n	;

" "     |
"\t"    |
"\r"	|
"\n"    ;

"\n\n"				return(PROMPT);

[^ \t\r\n]*                     return(JUNK);

%%
int yywrap(){
return;
}