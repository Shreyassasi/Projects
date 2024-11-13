#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 500

void read_reverse(FILE *file) {
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char buffer[MAX_LINE_LENGTH];  /*size of buffer same as */
	char *lines[1000]; //lines less than 1000
	int line_count = 0;

	while(fgets(buffer,sizeof(buffer),file)) {
		size_t len = strlen(buffer); //removinge any new line or empty line
		if(buffer[len -1]=='\n') {
			buffer[len-1] = '\0';
		}
		lines[line_count]=malloc(strlen(buffer)+1); //256 *4
		if(lines[line_count]!=NULL) {
			strcpy(lines[line_count],buffer);
			line_count++;
		}
	}
	for(int i =line_count-1; i>=0; i--) {
		// reverse_line(lines[i]);
		printf("%s\n",lines[i]);
		free(lines[i]);
	}
}

int main() {
	FILE *file = fopen("read.txt", "r");

	if (file == NULL) {
		perror("Error opening file");
		return 1;
	}
	read_reverse(file);

	fclose(file);
	return 0;
}
