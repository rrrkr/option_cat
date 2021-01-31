#include<stdio.h>   /* fprintf,snprintf,perror */
#include<stdlib.h>  /* exit */
#define _GNU_SOURCE /* getopt_long */
#include<getopt.h>  /* getopt_long */
#include<string.h>  /* strlen */

static void do_cat_ne(FILE *fp);
static void do_cat_n(FILE *fp);
static void do_cat_e(FILE *fp);
static void do_cat(FILE *fp);
static void die(const char *str);

#define TRUE 1
#define FALSE 0

int main(int argc,char *argv[])
{
	FILE *fp;
	int i;
	int opt;
	int number = FALSE;
	int show_ends = FALSE;

	static struct option const longopts[] = {
  	{"number"   ,no_argument, NULL, 'n'},
		{"show-ends",no_argument, NULL, 'e'},
		{"help"     ,no_argument, NULL, 'h'},
		{NULL       ,0          , NULL,  0 },
	};

	while((opt = getopt_long(argc,argv, "neh", longopts, NULL)) != -1) {
		switch(opt) {
			case 'n':
				number = TRUE;
				break;
			case 'e':
				show_ends = TRUE;
				break;
			case 'h':
				fprintf(stderr, "Usage: %s [-nE] [file ...]\n", argv[0]);
				exit(0);
		}
	}

	if(optind == argc) {
		if(number & show_ends) 
			do_cat_ne(stdin);
		else if(number)
			do_cat_n(stdin);
		else if(show_ends)
			do_cat_e(stdin);
		else
			do_cat(stdin);
	} else {
		int i;
		for(i = optind ; i < argc ; i++){
			FILE *fp;

			if((fp = fopen(argv[i], "r")) == NULL) die(argv[0]);
			if(number & show_ends)
				do_cat_ne(fp);
			else if(number)
				do_cat_n(fp);
			else if(show_ends)
				do_cat_e(fp);
			else
				do_cat(fp);

			if(fclose(fp) == EOF) die(argv[0]);
		}
	}
	exit(0);
}

#define LINELEN 2048

static void do_cat_ne(FILE *fp)
{
	char tmp_buf[LINELEN];
	char buf[LINELEN + 3];
	unsigned int count = 0;

	while(fgets(tmp_buf, sizeof tmp_buf, fp) != NULL){
		tmp_buf[strlen(tmp_buf) - 1] = '$';
		snprintf(buf, sizeof buf, "%d\t%s\n", ++count, tmp_buf);
		fputs(buf, stdout);
	}
}

static void do_cat_n(FILE *fp)
{
	char tmp_buf[LINELEN];
	char buf[LINELEN + 2];
	unsigned int count = 0;

	while(fgets(tmp_buf, sizeof tmp_buf, fp) != NULL){
		snprintf(buf, sizeof buf, "%d\t%s", ++count, tmp_buf);
		fputs(buf, stdout);
	}
}
static void do_cat_e(FILE *fp)
{
	char tmp_buf[LINELEN];
	char buf[LINELEN + 1];

	while(fgets(tmp_buf, sizeof tmp_buf, fp) != NULL){
		tmp_buf[strlen(tmp_buf) - 1] = '$';
		snprintf(buf, sizeof buf, "%s\n", tmp_buf);
		fputs(buf, stdout);
	}
}
static void do_cat(FILE *fp)
{
	char buf[LINELEN];
	while(fgets(buf, sizeof buf, fp) != NULL){
		fputs(buf, stdout);
	}
}
static void die(const char *str)
{
	perror(str);
	exit(1);
}
