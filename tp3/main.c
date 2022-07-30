#include "main.h"
#include "utils.h"
#include "options.h"



int main(int argc, char *argv[]) {
	int flag_a, flag_f, opt_ind;
	parseOptions(argc, argv, &opt_ind, &flag_a, &flag_f);
	// TODO use flag_a and flag_f
	//// printf("Argc: %d, opt_ind: %d, flag_a: %d\n", argc, opt_ind, flag_a);

	int n_arguments = argc - opt_ind;

	if (n_arguments == 0) {
		raise_error("Not enough arguments");
	}

	if (n_arguments == 1) {
		// List file or directory:

		const char* target_path = argv[argc-1];
		ResourceProps props = get_resource_props(target_path);

		if (props.type == S_IFLNK) {
			// Softlink
			// We must resolve a real file path from a link to file
			char* real_filename = readlink_again(target_path);
			props = get_resource_props(real_filename);
		}

		if (props.type == S_IFDIR) {
			print_file_attrs(props, target_path);
			list_dir(target_path);
		} 
		else if (props.type == S_IFREG) {
			// File or hard link
			print_file_attrs(props, target_path);
		} 
		else {
			raise_error("Impossible to show");
		}

		return 0;
	}

	// Copy all input files:

	const char* destination = argv[argc-1];

	ResourceProps destination_props = get_resource_props(destination);
	printf("Destination %s type %d\n", destination, destination_props.type);

	// The application can receive a symbolic link to the directory as an argument.
	// You can read this link and get your final destination
	if (destination_props.type == S_IFLNK) {
		char* real_filename = readlink_again(destination);
		destination_props = get_resource_props(real_filename);
	}

	// It is now clear that the target resource must be a directory.
	if (destination_props.type != S_IFDIR) {
		raise_error("Impossible to copy");
	} 

	// Add `/` in case path does not end with `/`
	// This transformation has a meaning for the next_filename variable
	if (destination[strlen(destination) - 1] != '/') {
		strcat((char*)destination, "/");
	}
	
	int i;
	ResourceProps existed_props, source_props;

	// Go through all inputed paths
	for (i = opt_ind; i < argc - 1; i++) {
		char* filename = get_filename(argv[i]);
		// next_filename will be a clone of the destination string, which will be expanded with the filename by concatenation
		char* next_filename = strdup(destination);
		// add the filename variable to the end of the next_filename variable
		strcat(next_filename, filename);
	
		source_props = get_resource_props(argv[i]);
		// Check if this file exists and get the properties of this file
		existed_props = get_resource_props(next_filename);

		int must_be_rewritten = resource_can_be_rewritten(source_props, existed_props);

		if (source_props.type == S_IFREG) {
			// need to overwrite existing file?
			if (must_be_rewritten > 0) {
				copy_file(argv[i], next_filename);
				populate_resource_attributes(next_filename, source_props);
				print_file_attrs(get_resource_props(next_filename), next_filename);
			}
		} else if (source_props.type == S_IFLNK) {
			if (must_be_rewritten > 0) {
				// the symlink() function does not work when the file already exists
				if (existed_props.type > -1 ) {
					// we must delete this file before we create a link with the same path
					unlink(next_filename);
				}
			
				char* real_filename = readlink_again(argv[i]);
				// The symlink() function is provided by <unistd.h>
				int op_status = symlink(real_filename, next_filename);
				
				if (op_status == -1){ 
					printf("Failed to copy link for file %s\n", argv[i]);
				} else {
					populate_resource_attributes(next_filename, source_props);
					print_file_attrs(get_resource_props(next_filename), next_filename);
				}
			}
		} else if (source_props.type == S_IFDIR) {
			dir_deep_copy(argv[i], source_props, next_filename, existed_props);
		}
		else {
			printf("Impossible to copy %s\n", argv[i]);
		}
	}

	return 0;
}
