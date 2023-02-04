#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"


/*** make response to the require***/
void response(char* buffer, int newsockfd, char* absolute_path) {
	
	const char space[2] = " ";
	char text[MAXCHAR] = "HTTP/1.0 404 Not Found\r\n";

	// store the pre string "GET". it will be checked later
	char check_valid_pre[MAXCHAR];
	strcpy(check_valid_pre, strtok(buffer, space));

	// get the path
	char str_path[MAXCHAR];
	strcpy(str_path, strtok(NULL, space));

	// concat the absolute path and file path to full_path
	char full_path[MAXCHAR];
	strcpy(full_path, absolute_path);
	strcat(full_path, str_path);

	// check if there is a string after the path
	char* name = strtok(NULL, "\r\n\0");

	if (name != NULL) {
		
		// check if the string is HTTP/1.0
		if((strcmp(name, "HTTP/1.0")) == 0) {

			// send the file
			send_file(full_path, check_valid_pre, newsockfd, str_path);

		// if not HTTP/1.0, return 404
		} else {
			int check1 = write(newsockfd, text, strlen(text));
			if(check1 <= 0) {
				close(newsockfd);
			}
		}
	
	// if there is no string after, return 404
	}else {
		int check2 = write(newsockfd, text, strlen(text));
		if(check2 <= 0) {
			close(newsockfd);
		}
	}
}



/***open the file and send file***/
void send_file(char* full_path, char* check_valid_pre, int newsockfd, char* str_path) {

	char text[MAXCHAR] = "HTTP/1.0 404 Not Found\r\n";

	// check if the input is valid
	FILE* fp;
	if ((strcmp(check_valid_pre, "GET") == 0)) {
		fp = fopen(full_path, "r");
	}

	// if invalid input or ../ case
	if ((!fp) || (strstr(full_path, "../") != NULL)) {
		int check3 = write(newsockfd, text, strlen(text));
		if(check3 <= 0) {
			close(newsockfd);
		}

	// if the file is able to access
	} else if (fp){
		char text_1[MAXCHAR] = "HTTP/1.0 200 OK\r\n";
		int check4 = write (newsockfd, text_1, strlen(text_1));
		if(check4 <= 0) {
			close(newsockfd);
		}
			
		// get the file type
		for(int i=0; i<strlen(full_path); i++) {

			// if the file has "."
			if (full_path[i] == '.') {
				send_content_type(newsockfd, str_path);
				break;

			// if the file does not has "." 
			} else if (i == strlen(full_path) - 1){
				char display[MAXCHAR];
				strcpy(display, "Content-type: application/octet-stream\r\n\r\n");
				int check5 = write(newsockfd, display, strlen(display));
				if(check5 <= 0) {
					close(newsockfd);
				}		
			}
		}

		// send file
		char new_buffer[EACH_READ];
		int size;
		while((size = fread(new_buffer, 1, EACH_READ, fp)) > 0) {
			int check6 = write(newsockfd, new_buffer, size);
			if(check6 <= 0) {
				close(newsockfd);
			}
		}
	}
}




void send_content_type(int newsockfd, char* str_path) {
	const char dot[2] = ".";
	char* str_type = strtok(str_path, dot);
	str_type  = strtok(NULL, dot);

	// check if the file type matches the 4 given file types
	char output[MAXCHAR];
	if(strcmp(str_type, "html") == 0) {
		strcpy(output, "Content-type: text/html\r\n\r\n");
	} else if (strcmp(str_type, "jpg") == 0) {
		strcpy(output, "Content-type: image/jpeg\r\n\r\n");
	} else if (strcmp(str_type, "css") == 0) {
		strcpy(output, "Content-type: text/css\r\n\r\n");
	} else if (strcmp(str_type, "js") == 0) {
		strcpy(output, "Content-type: text/javascript\r\n\r\n");
	} else {
		strcpy(output, "Content-type: application/octet-stream\r\n\r\n");
	}
	// send file back
	int check7 = write(newsockfd, output, strlen(output));
	if(check7 <= 0) {
		close(newsockfd);
	}
}