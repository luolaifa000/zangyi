#define DEFAULT_LOG_FILE "debug.log"

typedef struct {
    char *logfile;
} zconf;


zconf *init_config();