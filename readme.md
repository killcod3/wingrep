# wingrep

`wingrep` is a Python interface to a C library for searching files and directories in windows environmemnt, similar to the Unix `grep` tool. It provides efficient file searching capabilities by leveraging a shared library written in C.

## Features

- Search for patterns in files and directories.
- Supports various search options such as case-insensitive search, line numbers, word matching, and more.
- Handles binary files with customizable behavior.
- Displays context lines around matches.

## Installation

### Prerequisites

- Python 3.x
- GCC (for compiling the shared library)

### Compiling the Shared Library

Before using `wingrep`, you need to compile the shared library from the C source file. Navigate to the directory containing your `libgrep.c` file and run:

```sh
gcc -shared -o wingrep/libgrep.so -fPIC libgrep.c
```

### Installing the Package

To install the `wingrep` package, navigate to the root directory of the project (where `setup.py` is located) and run:

```sh
python setup.py install
```

Alternatively, you can use `pip` to install the package in editable mode:

```sh
pip install -e .
```

## Usage

After installation, you can use the `wingrep` command from the command line:

```sh
wingrep pattern path [options]
```

### Command Line Options

- `pattern`: The pattern to search for.
- `path`: The file or directory to search in.
- `-i`, `--ignore-case`: Perform case insensitive search.
- `-n`, `--line-number`: Show line numbers.
- `-v`, `--invert-match`: Invert the sense of matching.
- `-c`, `--count`: Count matching lines.
- `-l`, `--files-with-matches`: Print only names of files with matches.
- `-w`, `--word-regexp`: Match whole words only.
- `-x`, `--line-regexp`: Match whole lines only.
- `-b`, `--byte-offset`: Print the byte offset with output lines.
- `-A`, `--after-context`: Print `num` lines of trailing context after matching lines.
- `-bin`, `--binary-files`: Determine how to handle binary files (`binary`, `text`, `skip`).

### Example

Search for the pattern "example" in the current directory and its subdirectories, displaying line numbers and matching whole words only:

```sh
wingrep example . -n -w
```

## Project Structure

```
wingrep/
│
├── wingrep/
│   ├── __init__.py
│   ├── wingrep.py
│   ├── libgrep.so
│   └── libgrep.h
│
├── setup.py
└── README.md
```

## Development

### Adding New Features

To add new features or modify existing ones, follow these steps:

1. Update the `libgrep.c` file with the required changes.
2. Recompile the shared library:

    ```sh
    gcc -shared -o wingrep/libgrep.so -fPIC libgrep.c
    ```

3. Update `wingrep.py` if necessary to reflect the changes in the shared library.
4. Test the changes thoroughly before committing.

### Testing

To test the `wingrep` functionality, you can use various test files and directories. Ensure that you cover different cases such as:

- Case-sensitive and case-insensitive searches.
- Word and line matching.
- Handling of binary files.
- Counting matches and displaying context lines.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Author

Jawad Shafique
- Email: [shafiquejr@gmail.com](mailto:shafiquejr@gmail.com)
- GitHub: [killcod3](https://github.com/killcod3/wingrep)

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes. Ensure that your code follows the project's coding standards and includes appropriate tests.

## Acknowledgements

Special thanks to the open-source community for their invaluable resources and support.
