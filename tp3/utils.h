#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <utime.h>


//// static const int IS_SOCKET = 1;
#define BUF_SIZE 65536
#define RETURN_NOT_FOUND -1


// FR: add absolute path
typedef struct resource_props{
	off_t size;
	time_t last_modification;
	time_t last_access;
	mode_t mode;
	mode_t type;
	char* permissions;
} ResourceProps;
/* Where ResourceProps is an aliase for the resource_props structure */

void raise_error(const char* message);
void print_file_attrs(ResourceProps props, const char* path);
struct resource_props get_resource_props(const char* path);
char* readlink_again(const char* filename);
void list_dir(const char* path);
void dir_deep_copy(char* soure_dir, ResourceProps source_props, char* destination, ResourceProps existed_props);
char* get_filename(char* path);

int copy_file(char* source, char* dest);
void populate_resource_attributes(char* filename, ResourceProps props);
int resource_can_be_rewritten(ResourceProps source_props, ResourceProps existed_props);
char* join_path(char* base_path, char* filename);