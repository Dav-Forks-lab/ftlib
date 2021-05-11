# ftlib

file-tracer is a C library for local file searching.

## Installation

Just clone the directory and include the C file in your project

## Usage/Docs

```C
#include "ftlib.c"

int main()
{
    // Create the structure object
    Folder* f_obj = malloc(sizeof(Folder));  
    
    // Initialize structure
    init(f_obj, file_name_to_search);  
    
    // Search the filename
    find_file(f_obj);  
    
    // Insert filters in an array
    set_filter(f_obj, filter_array, filter_array_lenght);  
    
    // Fill a given array with filtered result
    apply_filter(f_obj, filtered_result_array, filtered_result_array_index);
    
    // Change the filename to be searched
    change_filename(f_obj, new_filename);

    // Set a new root direcotry
    change_root_direcotory(f_obj, new_root_directory);

    // Print result array (made for devs)
    print(f_obj);
}
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://github.com/Zambo-dev/ftlib/blob/master/LICENSE)
