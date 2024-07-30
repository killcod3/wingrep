from setuptools import setup, find_packages

setup(
    name='wingrep',
    version='1.0.0',
    description='A Python interface to a C library for searching files and directories, similar to grep.',
    author='Jawad Shafique',
    author_email='shafiquejr@gmail.com',
    url='https://github.com/killcod3/wingrep',  # Replace with your project URL
     package_data={
        'wingrep': ['*.so'],  # Include shared object and dependancy
    },
    data_files=[
        ('', ['wingrep/libgrep.so']),  # Include shared library
    ],
    entry_points={
        'console_scripts': [
            'wingrep=wingrep:main',  # Replace 'grep_interface' with the name of your script
        ],
    },
    include_package_data=True,
    zip_safe=True,
    license = "MIT license",
)
