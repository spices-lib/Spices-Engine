'''
@files Statics.py.
@brief Statics some information.
@author Spices.
'''

import os
from itertools import (takewhile, repeat)

'''
@brief Files basic root folder.
'''
rootfolder: str = 'D:/OpenGLProjects/Spices-Engine'

'''
@brief Files with those extensions can be counted.
'''
extensions: dict = {
    'c': 1,
    'h': 1,
    'cpp': 1,
    'hpp': 1,
    'lua': 1,
    'py': 1,
    'material': 1,
    'glsl': 1,
    'mesh': 1,
    'task': 1,
    'vert': 1,
    'geom': 1,
    'frag': 1,
    'comp': 1,
    'rgen': 1,
    'rchit': 1,
    'rmiss': 1,
    'bat': 1,
}

'''
@brief Files in those folders can not be counted.
'''
ignorefolders: dict = {
    'vendor': 1,
    '.git': 1,
    '.idea': 1,
    '.vs': 1,
    'bin': 1,
    'bin-int': 1,
    'venv': 1
}

'''
@brief Counted rows.
'''
nrows: int = 0

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
    global ignorefolders
    global nrows

    for filepath, dirnames, filenames in os.walk(file_dir):
        for filename in filenames:

            ignore = False
            for ignorefolder in ignorefolders:
                if filepath.find(ignorefolder) != -1:
                    ignore = True
                    break

            if ignore:
                continue

            fullname = os.path.join(filepath, filename)
            if extensions.get(os.path.splitext(filename)[-1][1:]) is not None:
                nrows += iter_count(os.path.join(filepath, filename))

def main():

    '''
    @brief Iter all files and count rows and print result.
    '''

    global rootfolder

    iter_files(rootfolder)
    print(nrows)

if __name__ == '__main__':
    main()