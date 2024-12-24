'''
@files Build.py.
@brief Build Spices Solution.
@author Spices.
'''

import subprocess
import argparse
import os

'''
@brief vswhere path.
'''
vswhere_path : str = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

def find_msbuild():

    '''
    @brief Find MSBuild path.
    @return Returns MSBuild path.
    '''

    global vswhere_path
    
    result = subprocess.run([
        vswhere_path, 
        "-latest", 
        "-products", 
        "*", 
        "-requires", 
        "Microsoft.Component.MSBuild", 
        "-find", 
        "MSBuild\**\Bin\MSBuild.exe"], 
        capture_output = True, 
        text = True
    )

    if result.returncode == 0 and result.stdout:
        return result.stdout.strip()
    else:
        raise Exception("Could not find MSBuild")

def main():

    '''
    @brief Build Spices Solution with configuration.
    '''

    parser = argparse.ArgumentParser(description='Build Spices Solution')
    parser.add_argument('--target'       , type=str, required=True, help='target solution, e.g. C:/')
    parser.add_argument('--configuration', type=str, required=True, help='target solution build configuration, e.g. C:/')
    parser.add_argument('--platform'     , type=str, required=True, help='target solution build platform, e.g. C:/')
    args = parser.parse_args()

    if not os.path.isfile(args.target):
        raise Exception('Invalid argument for --target: {}'.format(args.target))

    msbuild_path  = find_msbuild()
    solution_path = args.target
    
    # Solution Build Command
    build_command = [
        msbuild_path,
        solution_path,
        "/p:Configuration=" + args.configuration,
        "/p:Platform=" + args.platform,
        "/v:m"  # Set minimal detail level
    ]

    # Execute Build
    process = subprocess.Popen(build_command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
 
    # Output Build result
    try:
        for line in process.stdout:
            print(line, end='')
    except KeyboardInterrupt:
        print("\nBuild interrupted by user.")
        process.terminate()
        process.wait()
        return

    # Wait for Build
    process.wait()

    if process.returncode == 0:
        print("Build Succeed")
    else:
        print("Build Failed, Error Code:", process.returncode)

if __name__ == '__main__':
    main()