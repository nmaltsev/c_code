#include "utils.h"

void raise_error(const char* message){
	printf("%s\n", message);
	exit(EXIT_FAILURE);
}

char* get_resource_permissions(mode_t mode) {
	const char* chars = "rwxrwxrwx";
	char* buf = malloc(10 * sizeof(char));

	for(size_t i = 0; i < 9; i++) {
		buf[i] = (mode & (1 << (8 - i))) ? chars[i] : '-';
	}
	buf[9] = '\0';
	
	return buf;
}

void print_file_attrs(ResourceProps props, const char* path) {
	// strftime() depends on <time.h>
	// the length of a sentence like "Sat Oct 16 20:34:30 2020" will be 25
	int max_date_len = 25;
	char* time_str = malloc(sizeof(char) * (max_date_len + 1));
	strftime(time_str, max_date_len, "%c", localtime(&props.last_modification));

	char file_type = ' ';
	if (props.type == S_IFDIR) {
		file_type = 'd';
	}
	else if (props.type == S_IFREG) {
		file_type = '-';
	}
	else if (props.type == S_IFLNK) {
		file_type = 'l';
	}
		
	// The file size will be displayed with 10 leading digits
	printf(
		"%c%s %10lld %s %s\n",
		file_type,
		get_resource_permissions(props.mode),
		(long long)props.size,
		time_str,
		path
	);
}


struct resource_props get_resource_props(const char* path) {
	/*
		https://man7.org/linux/man-pages/man2/stat.2.html
		https://man7.org/linux/man-pages/man7/inode.7.html
		returns:
			S_IFSOCK   0140000   socket
			S_IFLNK    0120000   symbolic link
			S_IFREG    0100000   regular file
			S_IFBLK    0060000   block device
			S_IFDIR    0040000   directory
			S_IFCHR    0020000   character device
			S_IFIFO    0010000   FIFO
	*/
	struct stat data;
	int status = lstat(path, &data);
	struct resource_props out;
	
	if (status != 0) {
		out = (struct resource_props){
			.size = -1,
			.last_modification = -1,
			.last_access = -1,
			.mode = -1,
			.type = -1,
		};
		return out;
	}
	
	out = (struct resource_props){
		.size = data.st_size,
		.last_modification = data.st_mtime,
		.last_access = data.st_atime,
		.mode = data.st_mode,
		.type = data.st_mode & S_IFMT
	};

	return out;
}


char* readlink_again(const char* filename) {
	size_t size = 4;
	char* buf = NULL;

	while(1) {
		buf = realloc(buf, size * 2);
		size *= 2;
		// The readlink() function is provided by unistd.h
		ssize_t nchars = readlink(filename, buf, size);

		if (nchars < 0){
			free(buf);
			return NULL;
		}

		if (nchars < size) {
			return buf;
		}
	}
}

void list_dir(const char* root_path){ 
	// Depends on <dirent.h>

	char* filename, *path;
	struct dirent* dir_data = NULL;
	DIR* rep = opendir(root_path);
	ResourceProps props;
	
	if (rep == NULL) {
		printf("Can't open dir %s\n", root_path);
		return;
	}
	
	while ((dir_data = readdir(rep)), dir_data != NULL) {
		filename = (char*)dir_data->d_name;

		if (filename[0] == '.') {
			continue;
		}
		
		path = join_path((char*)root_path, filename);
		props = get_resource_props(path);
		
		if (props.type == S_IFDIR) {
			print_file_attrs(props, path);
			list_dir(path);
		}
		else if (props.type == S_IFREG || props.type == S_IFLNK) {
			// File or hard link
			print_file_attrs(props, path);
		}
		else {
			printf("not file, link or dir: `%s` \n", path);
		}
	}

	if (closedir(rep) == -1) {	
		perror("Can't close dir");
	}
}


int last_index_of(char* str, char ch, int start_pos) {
	int pos = strlen(str) - 1;
	if (start_pos <= pos && start_pos >= 0) pos = start_pos;
	for (; pos >= 0; pos--) {
		if (str[pos] == ch) return pos;
	}
	return RETURN_NOT_FOUND;
}

char* get_filename(char* path) {
	/*
		'main.c' => 'main.c'
		'../abc/main.c' => 'main.c'
	*/
	int pos = last_index_of(path, '/', -1);
	// returns the original string if there is no '/' character
	if (pos < 0) return path;

	// Getting a substring from a string like '../<substr>'
	// The size of the substring that will store the filename
	int substr_len = strlen(path) - pos - 1;
	// Memory for substring
	char* out = malloc(sizeof(char) * substr_len);
	// copy a substring from pos + 1 to the end of the string
	strncpy(out, path + pos + 1, substr_len);
	return out;
}

int copy_file(char* source, char* destination) {
	FILE *src, *dst;
	size_t in, out;
	char *buf = malloc(sizeof(char) * BUF_SIZE);
	src = fopen(source, "rb");
	dst = fopen(destination, "wb");
	if (src == NULL || dst < 0) return EXIT_FAILURE;
	while (1) {
		in = fread(buf, sizeof(char), BUF_SIZE, src);
		if (in == 0) break;
		out = fwrite(buf, sizeof(char), in, dst);
		if (out == 0) break;
	}
	fclose(src);
	fclose(dst);

	return EXIT_SUCCESS;
}


void populate_resource_attributes(char* filename, ResourceProps props) {
	// depends on <utime.h>
	struct utimbuf source_time = {
		.actime = props.last_access,
		.modtime = props.last_modification,
	};

	chmod(filename, props.mode);
	utime(filename, &source_time);
}


char* join_path(char* base_path, char* filename) {
	size_t base_path_len = strlen(base_path);
	int slash_is_missed = base_path[base_path_len - 1] != '/' ? 1 : 0;
	char* path = malloc(base_path_len + strlen(filename) + slash_is_missed + 1);
	strcpy(path, base_path);
	if (slash_is_missed == 1) strcat(path, "/");
	strcat(path, filename);
	return path;
}


void dir_deep_copy(char* source_path, ResourceProps source_props, char* destination_path, ResourceProps existed_props) {
	if (existed_props.type == -1) {
		int op_status = mkdir(destination_path, source_props.mode);

		if (op_status != EXIT_SUCCESS) {
			perror("Can't create directory");
		} else {
			print_file_attrs(get_resource_props(destination_path), destination_path);
		}
	}

	char* filename;
	struct dirent* dir_data = NULL;
	DIR* rep = opendir(source_path);			
	
	if (rep == NULL) {
		printf("Can't open dir %s\n", source_path);
		// perror("Can't open dir"); // perror() also types a reason
	}
	
	while ((dir_data = readdir(rep)), dir_data != NULL) {
		filename = (char*)dir_data->d_name;

		if (filename[0] == '.') continue;
		
		char* item_path = join_path(source_path, filename);
		char* destination_item_path = join_path(destination_path, filename);
		ResourceProps item_props = get_resource_props(item_path); 
		ResourceProps destination_props = get_resource_props(destination_item_path);
		
		if (item_props.type == S_IFDIR) {
			dir_deep_copy(item_path, item_props, destination_item_path, destination_props);
		}
		else if (item_props.type == S_IFREG || item_props.type == S_IFLNK) {
			int must_be_rewritten = resource_can_be_rewritten(item_props, destination_props);

			if (must_be_rewritten > 0) {
				if (item_props.type == S_IFREG) { 
					// for file or hardlink
					copy_file(item_path, destination_item_path);
				} else { 
					// For soft link
					if (destination_props.type > -1 ) {
						// we have to delete this file before creating a link with the same path
						unlink(destination_item_path);
					}
				
					char* real_filename = readlink_again(item_path);
					// The symlink() function is provided by <unistd.h>
					int op_status = symlink(real_filename, destination_item_path);
					
					if (op_status == -1){ 
						printf("Failed to copy link for file %s\n", real_filename);
					}
				}
				
				populate_resource_attributes(destination_item_path, item_props);
				print_file_attrs(get_resource_props(destination_item_path), destination_item_path);
			}
		}
		else {
			printf("not file, link or dir: `%s` \n", item_path);
		}
	}

	if (closedir(rep) == -1) {	
		perror("Can't close dir");
	}
}


int resource_can_be_rewritten(ResourceProps source_props, ResourceProps existed_props) {
	// resource with the same name does not exist
	if (existed_props.type < 0) return 1;

	if (
		source_props.size != existed_props.size &&
		source_props.last_modification > existed_props.last_modification
	) return 1;

	return 0;
}
