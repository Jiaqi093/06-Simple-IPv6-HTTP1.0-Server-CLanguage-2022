#define MAXCHAR 2048
#define EACH_READ 1024
#define IMPLEMENTS_IPV6
#define MULTITHREADED

typedef struct {
    int newsockfd;
    char* absolute_path;
} data_t;

void response(char* buffer, int newsockfd, char* absolute_path);
void send_file(char* full_path, char* check_valid_pre, int newsockfd, char* str_path);
void send_content_type(int newsockfd, char* str_path);