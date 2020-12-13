#!/usr/bin/python

import sys
import os
import shutil
from datetime import datetime

# synchronize folders
def rcopy(input_folder, output_folder):

    # create folder
    if not os.path.exists(output_folder):
        os.mkdir(output_folder)
        
    # list input
    input_files = []
    
    for entry in os.scandir(input_folder):
        input_files.append(entry)

    # list output
    output_files = []

    for entry in os.scandir(output_folder):
        output_files.append(entry)

    # remove deleted
    for output_entry in output_files:
        exists = False;
        
        for input_entry in input_files:
            if output_entry.name == input_entry.name:
                exists = True
                break
                
        if not exists:
            print('remove ' + output_entry.path)
            if output_entry.is_file():
                os.remove(output_entry)
            else:
                shutil.rmtree(output_entry)
    
    # copy files
    for entry in input_files:
        current_path = os.path.join(output_folder, entry.name)
        
        # check if file don't exists or if modification date is different
        if entry.is_file():
            if not os.path.exists(current_path) or os.path.getmtime(entry.path) > os.path.getmtime(current_path):
                print('copy ' + entry.path + ' -> ' + current_path)
                shutil.copy(entry.path, output_folder)

        elif not entry.name.startswith('.'):
            rcopy(entry.path, current_path)

# execute 
if __name__ == "__main__":
    source = sys.argv[1]
    destination = sys.argv[2]

    print('sync ' + source + ' to ' + destination)
    rcopy(source, destination)
    
    # generate mock
    with open("last_copy.h", mode="w") as file:
        file.write(datetime.now().strftime("%d/%m/%Y %H:%M:%S"))