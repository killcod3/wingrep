# wingrep.py
import argparse
import os
import ctypes
import sys

# Load the shared library
try:
    libgrep = ctypes.CDLL('libgrep.so')
except OSError as e:
    print(f"Error loading shared library: {e}", file=sys.stderr)
    sys.exit(1)

# Define argument types and return types for the functions in the shared library
libgrep.search_line.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_bool, ctypes.c_bool, ctypes.c_bool]
libgrep.search_line.restype = ctypes.c_bool

libgrep.grep_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_bool, ctypes.c_bool, ctypes.c_bool,
                              ctypes.c_bool, ctypes.c_bool, ctypes.c_bool, ctypes.c_bool, ctypes.c_bool,
                              ctypes.c_int, ctypes.c_char_p]
libgrep.grep_file.restype = None

libgrep.grep_directory.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_bool, ctypes.c_bool, ctypes.c_bool,
                                   ctypes.c_bool, ctypes.c_bool, ctypes.c_bool, ctypes.c_bool, ctypes.c_bool,
                                   ctypes.c_int, ctypes.c_char_p]
libgrep.grep_directory.restype = None

def main():
    parser = argparse.ArgumentParser(description="Search for a pattern in a file or directory.")
    parser.add_argument('pattern', help="The pattern to search for.")
    parser.add_argument('path', help="The file or directory to search in.")
    parser.add_argument('-i', '--ignore-case', action='store_true', help="Perform case insensitive search.")
    parser.add_argument('-n', '--line-number', action='store_true', help="Show line numbers.")
    parser.add_argument('-v', '--invert-match', action='store_true', help="Invert the sense of matching.")
    parser.add_argument('-c', '--count', action='store_true', help="Count matching lines.")
    parser.add_argument('-l', '--files-with-matches', action='store_true', help="Print only names of files with matches.")
    parser.add_argument('-w', '--word-regexp', action='store_true', help="Match whole words only.")
    parser.add_argument('-x', '--line-regexp', action='store_true', help="Match whole lines only.")
    parser.add_argument('-b', '--byte-offset', action='store_true', help="Print the byte offset with output lines.")
    parser.add_argument('-A', '--after-context', type=int, default=0, help="Print num lines of trailing context after matching lines.")
    parser.add_argument('-bin', '--binary-files', choices=['binary', 'text', 'skip'], default='binary', help="Determine how to handle binary files.")
    
    args = parser.parse_args()

    pattern = args.pattern.encode('utf-8')
    path = args.path.encode('utf-8')
    binary_files = args.binary_files.encode('utf-8')

    if not os.path.exists(args.path):
        print(f"Error: The path '{args.path.decode('utf-8')}' does not exist.", file=sys.stderr)
        sys.exit(1)

    try:
        if os.path.isdir(args.path):
            libgrep.grep_directory(path, pattern, args.ignore_case, args.line_number, args.invert_match, args.count,
                                   args.files_with_matches, args.word_regexp, args.line_regexp, args.byte_offset,
                                   args.after_context, binary_files)
        else:
            libgrep.grep_file(path, pattern, args.ignore_case, args.line_number, args.invert_match, args.count,
                              args.files_with_matches, args.word_regexp, args.line_regexp, args.byte_offset,
                              args.after_context, binary_files)
    except Exception as e:
        print(f"An error occurred: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()
