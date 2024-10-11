'''
@files Statics.py.
@brief Statics some information.
@author Spices.
'''

import os
import argparse
from itertools import (takewhile, repeat)

'''
@brief Files basic root folder.
'''
rootFolder: str = ''

'''
@brief Files with those extensions can be counted.
'''
extensions: dict = {
    'c':        1,
    'h':        1,
    'cpp':      1,
    'hpp':      1,
    'lua':      1,
    'py':       1,
    'material': 1,
    'glsl':     1,
    'mesh':     1,
    'task':     1,
    'vert':     1,
    'geom':     1,
    'frag':     1,
    'comp':     1,
    'rgen':     1,
    'rchit':    1,
    'rmiss':    1,
    'bat':      1,
}

'''
@brief Files in those folders can not be counted.
'''
ignoreFolders: dict = {
    'spv':      1,
    'vendor':   1,
    '.git':     1,
    '.idea':    1,
    '.vs':      1,
    'bin':      1,
    'bin-int':  1,
    'venv':     1,
}

'''
@brief Counted rows.
'''
nRows: int = 0

'''
@brief Counted files.
'''
nFiles: int = 0

def iter_count(file_name: str):

    '''
    @brief calculate file rows.
    @param[in] file_name File's full name.
    @return Returns file rows.
    '''

    buffer = 1024 * 1024
    with open(file_name, encoding='gb18030', errors='ignore') as f:
        buf_gen = takewhile(lambda x: x, (f.read(buffer) for _ in repeat(None)))
        n = sum(buf.count('\n') for buf in buf_gen) + 1
        return n

def iter_files(file_dir: str):

    '''
    @brief Iter all files and count rows.
    @param[in] file_dir File's folder.
    '''

    global extensions
    global ignoreFolders
    global nRows
    global nFiles

    for filepath, dirnames, filenames in os.walk(file_dir):
        for filename in filenames:

            ignore = False
            for ignoreFolder in ignoreFolders:
                if filepath.find(ignoreFolder) != -1:
                    ignore = True
                    break

            if ignore:
                continue

            fullname = os.path.join(filepath, filename)
            if extensions.get(os.path.splitext(filename)[-1][1:]) is not None:
                nFiles += 1
                nRows += iter_count(os.path.join(filepath, filename))

def main():

    '''
    @brief Iter all files and count rows and print result.
    '''

    global rootFolder

    parser = argparse.ArgumentParser(description='Statistics Spices Project')
    parser.add_argument('--rootFolder', type=str, required=True, help='root folder path, e.g. C:/')
    args = parser.parse_args()

    if not os.path.isdir(args.rootFolder):
        raise Exception('Invalid argument for --rootFolder: {}'.format(args.rootFolder))

    rootFolder = args.rootFolder
    iter_files(rootFolder)
    print("Spices Project Statistics:")
    print("nRows : %d" % nRows)
    print("nFiles: %d" % nFiles)

if __name__ == '__main__':
    main()