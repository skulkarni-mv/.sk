#!/usr/bin/python3

import time
import webbrowser
import sys
import argparse

###################################
import os
import glob
from datetime import datetime, timedelta

import csv
from colorama import Fore
###################################

delay_time_pageload = 60	# seconds time given for each page to load

def compare_dates(from_date_str, till_date_str):
    date_format = '%Y-%m-%d'

    try:
        from_date = datetime.strptime(from_date_str, date_format)	        # Convert string dates to datetime objects
        till_date = datetime.strptime(till_date_str, date_format)

        if till_date >= from_date:					        # Compare the dates
            print("Till/End date is greater than or equal to From date.")
            return
        else:
            print(Fore.RED, "Error. Till/End date is not greater than or equal to From date. Exiting...", Fore.RESET)
            sys.exit(1)
    except ValueError as e:
        print(Fore.RED, f"Error parsing dates: {e}", Fore.RESET)
        sys.exit(1)
#enddef


def count_lines_csv(file_path):
    try:
        file = open(file_path, mode='r', newline='', encoding='utf-8')
        reader = csv.reader(file)
        line_count = sum(1 for _ in reader)
        return line_count

    except FileNotFoundError:
        print(Fore.RED, f"File not found to count: {file_path}", Fore.RESET)
    except Exception as e:
        print(Fore.RED, f"Error reading file to count: {e}", Fore.RESET)
    finally:
        file.close()		# Explicitly close the file
#enddef


def delete_today_date_csv_files(date_format, today_date, downloads_folder):	# DELETE the Existing 'bugs-{today_date}*.csv' files

    file_pattern = os.path.join(downloads_folder, f'bugs-{today_date}*.csv')	# Define the pattern for the file names to be deleted
    files_to_delete = glob.glob(file_pattern)					# Use glob to find files matching the pattern

    for file_path in files_to_delete:						# Delete each file found
        try:
            os.remove(file_path)
            print(Fore.GREEN, f"Deleted file: {file_path} \n", Fore.RESET)
        except Exception as e:
            print(Fore.RED, f"Error deleting file {file_path}: {e} \n", Fore.RESET)

    if not files_to_delete:
        print(Fore.RED, "No files matched the pattern to Delete. \n", Fore.RESET)
#enddef


########################## Main #############################

print(Fore.CYAN, "\n Usage: ./weblinks.py from_date(yyyy-mm-dd) till_date(yyyy-mm-dd) \n", Fore.RESET)

try:
    parser = argparse.ArgumentParser()
    parser.add_argument("DateFrom", type=str)
    parser.add_argument("DateTill", type=str)
    args = parser.parse_args()

    print(Fore.YELLOW ,"Argument passed: ", sys.argv[1], sys.argv[2], "\n", Fore.RESET)

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)


from_date_inp = sys.argv[1]
till_date_inp  = sys.argv[2]

compare_dates(from_date_inp, till_date_inp)

date_format = '%Y-%m-%d'
today_date = datetime.now().strftime(date_format)
downloads_folder = os.path.expanduser('~/Downloads')


from_date = datetime.strptime(from_date_inp, date_format)
till_date = datetime.strptime(till_date_inp, date_format)

from_date_query = from_date
till_date_query = till_date + timedelta(days=1)

from_date_query = from_date_query.strftime(date_format)
till_date_query = till_date_query.strftime(date_format)


print("------------------------------------------------------------")
delete_today_date_csv_files(date_format, today_date, downloads_folder)
print("------------------------------------------------------------")


#########################--- 1-Open_Still_bugs ---------------------------------------------------------------------

time.sleep(2)

now = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
webbrowser.open(f'file://Still_Open_as_of_Now_{now}')
time.sleep(1)

#  Still Open as of Now / UnResolved Yet
webbrowser.open('http://bugz.mvista.com/buglist.cgi?keywords=Security;keywords_type=allwords;bug_status=NEW;bug_status=ASSIGNED;bug_status=IN_REVIEW;bug_status=SYNC_REQ;bug_status=REOPENED;component=Kernel;component=Toolchain;component=userland;product=Carrier%20Grade;product=CentOS;product=CGX%202.0;product=CGX%202.2;product=CGX%202.4;product=CGX%202.6;product=CGX%203.1;product=CGX%204.0;product=CGX%205.0;product=Kubernetes;product=Rocky;product=Ubuntu')
time.sleep(1)

### RENAME FILE

old_file_name = f'bugs-{today_date}.csv'					# Construct the original and new file paths
old_file_path = os.path.join(downloads_folder, old_file_name)

for i in range(delay_time_pageload):						# takes time to load the data
    time.sleep(1)
    if os.path.exists(old_file_path):						# break from sleep, if the file is downloaded & exists
        time.sleep(1)
        break

lines=0
if os.path.exists(old_file_path):
    lines = count_lines_csv(old_file_path)
    lines = lines - 1

new_file_name = f'csv_dnld_{from_date.strftime(date_format)}_TO_{till_date.strftime(date_format)}_1-Open_Still_bugs-{lines}.csv'
new_file_path = os.path.join(downloads_folder, new_file_name)


if os.path.exists(old_file_path):						# Check if the old file exists
    try:
        os.rename(old_file_path, new_file_path)				        # Rename the file
        print(f"Renamed file: {old_file_path} to {new_file_path}")

    except Exception as e:
        print(Fore.RED, f"Error renaming file: {e}", Fore.RESET)
else:
    print(Fore.RED, f"\n\t\t\t\t File not found to rename: {old_file_path}", Fore.RESET)

    manual_file_name = f'csv_dnld_{from_date.strftime(date_format)}_TO_{till_date.strftime(date_format)}_1-Open_Still_bugs-00.csv'
    file = open(os.path.join(downloads_folder, manual_file_name), 'w')
    file.close()
    print(f"Created file: {os.path.join(downloads_folder, manual_file_name)}")

#########################--- 2-CreatedNEW_bugs ---------------------------------------------------------------------

time.sleep(2)

webbrowser.open(f'file://Created_Between_"{from_date_inp}"_and_"{till_date_inp}"')
time.sleep(1)

# Created between Specified Duration
webbrowser.open(f'http://bugz.mvista.com/buglist.cgi?keywords=Security;keywords_type=allwords;chfieldfrom={from_date_query};chfieldto={till_date_query};chfield=%5BBug%20creation%5D;component=Kernel;component=Toolchain;component=userland;product=Carrier%20Grade;product=CentOS;product=CGX%202.0;product=CGX%202.2;product=CGX%202.4;product=CGX%202.6;product=CGX%203.1;product=CGX%204.0;product=CGX%205.0;product=Kubernetes;product=Rocky;product=Ubuntu')
time.sleep(1)

### RENAME FILE

old_file_name = f'bugs-{today_date}.csv'					# Construct the original and new file paths
old_file_path = os.path.join(downloads_folder, old_file_name)

for i in range(delay_time_pageload):						# takes time to load the data
    time.sleep(1)
    if os.path.exists(old_file_path):						# break from sleep, if the file is downloaded & exists
        time.sleep(1)
        break

lines=0
if os.path.exists(old_file_path):
    lines = count_lines_csv(old_file_path)
    lines = lines - 1

new_file_name = f'csv_dnld_{from_date.strftime(date_format)}_TO_{till_date.strftime(date_format)}_2-CreatedNEW_bugs-{lines}.csv'
new_file_path = os.path.join(downloads_folder, new_file_name)


if os.path.exists(old_file_path):						# Check if the old file exists
    try:
        os.rename(old_file_path, new_file_path)				        # Rename the file
        print(f"Renamed file: {old_file_path} to {new_file_path}")

    except Exception as e:
        print(Fore.RED, f"Error renaming file: {e}", Fore.RESET)
else:
    print(Fore.RED, f"\n\t\t\t\t File not found to rename: {old_file_path}", Fore.RESET)

    manual_file_name = f'csv_dnld_{from_date.strftime(date_format)}_TO_{till_date.strftime(date_format)}_2-CreatedNEW_bugs-00.csv'
    file = open(os.path.join(downloads_folder, manual_file_name), 'w')
    file.close()
    print(f"Created file: {os.path.join(downloads_folder, manual_file_name)}")

#########################--- 3-Res_Within_bugs ---------------------------------------------------------------------

time.sleep(2)

webbrowser.open(f'file://Resolved_Between_"{from_date_inp}"_and_"{till_date_inp}"')
time.sleep(1)

# Resolved between Specified Duration
webbrowser.open(f'http://bugz.mvista.com/buglist.cgi?keywords=Security;keywords_type=allwords;chfieldfrom={from_date_query};chfieldto={till_date_query};chfield=bug_status;chfieldvalue=RESOLVED;bug_status=RESOLVED;bug_status=CLOSED;component=Kernel;component=Toolchain;component=userland;product=Carrier%20Grade;product=CentOS;product=CGX%202.0;product=CGX%202.2;product=CGX%202.4;product=CGX%202.6;product=CGX%203.1;product=CGX%204.0;product=CGX%205.0;product=Kubernetes;product=Rocky;product=Ubuntu')
time.sleep(1)

### RENAME FILE

old_file_name = f'bugs-{today_date}.csv'					# Construct the original and new file paths
old_file_path = os.path.join(downloads_folder, old_file_name)

for i in range(delay_time_pageload):						# takes time to load the data
    time.sleep(1)
    if os.path.exists(old_file_path):						# break from sleep, if the file is downloaded & exists
        time.sleep(1)
        break

lines=0
if os.path.exists(old_file_path):
    lines = count_lines_csv(old_file_path)
    lines = lines - 1

new_file_name = f'csv_dnld_{from_date.strftime(date_format)}_TO_{till_date.strftime(date_format)}_3-Res_Within_bugs-{lines}.csv'
new_file_path = os.path.join(downloads_folder, new_file_name)


if os.path.exists(old_file_path):						# Check if the old file exists
    try:
        os.rename(old_file_path, new_file_path)				        # Rename the file
        print(f"Renamed file: {old_file_path} to {new_file_path}")

    except Exception as e:
        print(Fore.RED, f"Error renaming file: {e}", Fore.RESET)
else:
    print(Fore.RED, f"\n\t\t\t\t File not found to rename: {old_file_path}", Fore.RESET)

    manual_file_name = f'csv_dnld_{from_date.strftime(date_format)}_TO_{till_date.strftime(date_format)}_3-Res_Within_bugs-00.csv'
    file = open(os.path.join(downloads_folder, manual_file_name), 'w')
    file.close()
    print(f"Created file: {os.path.join(downloads_folder, manual_file_name)}")

#########################-----------------------------------------------------------------------------------------------------

