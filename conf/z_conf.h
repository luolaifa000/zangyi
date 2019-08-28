#define DEFAULT_LOG_FILE "debug.log"

typedef struct {
    char *logfile;
} z_conf;


z_conf *init_config();