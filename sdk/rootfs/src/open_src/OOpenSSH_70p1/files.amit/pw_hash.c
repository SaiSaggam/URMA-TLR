#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int *pw_encrypt(char *cipher, const char *clear, const char *salt)
{
		char *cp;

		cp = (char *) crypt(clear, salt);
		strncpy(cipher, cp, 128);
		return 0;
}


int main (int argc, char* argv[]){

	char passwd_clear[32];
	char passwd_cipher[256];
	char sal[4];
	FILE *f;
	
	bzero(passwd_clear, sizeof(passwd_clear));
	bzero(passwd_cipher, sizeof(passwd_cipher));
	bzero(sal, sizeof(sal));

	strcpy(passwd_clear, (argv[1]) );

	sprintf(sal,"$1$");
	pw_encrypt(passwd_cipher,(const char *)passwd_clear,(const char *)sal);
	//printf("Passwd:%s\n", passwd_clear);
	printf("%s", passwd_cipher);
	
	f = fopen("hash", "w");
	
	if(f){
		fprintf(f,"%s",passwd_cipher);
		fclose(f);
	}else{
		printf("open file %s fail\n", "hash");
	}
	
	return 0;
}
