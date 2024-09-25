#!/usr/bin/python3

import time
import webbrowser
import sys
import argparse

###################################
import os
import glob
from datetime import datetime, timedelta
import pytz

import csv
from colorama import Fore
###################################

delay_time_pageload = 30	# seconds time given for each page to load


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
        print(Fore.RED, "No files matched the pattern 'bugs-{today_date}*.csv' to Delete. \n", Fore.RESET)

####

    file_pattern = os.path.join(downloads_folder, 'csv_dnld_Created_bugs.csv')	# Define the pattern for the file names to be deleted
    files_to_delete = glob.glob(file_pattern)					# Use glob to find files matching the pattern

    for file_path in files_to_delete:						# Delete each file found
        try:
            os.remove(file_path)
            print(Fore.GREEN, f"Deleted file: {file_path} \n", Fore.RESET)
        except Exception as e:
            print(Fore.RED, f"Error deleting file {file_path}: {e} \n", Fore.RESET)

    if not files_to_delete:
        print(Fore.RED, "No files matched the pattern 'csv_dnld_Created_bugs.csv' to Delete. \n", Fore.RESET)

#enddef


########################## Main #############################

print(Fore.CYAN, "\n Usage: ./weblinks.py bug_creation_date(yyyy-mm-dd) \n", Fore.RESET)

try:
    parser = argparse.ArgumentParser()
    parser.add_argument("BugCreationDate", type=str)
    args = parser.parse_args()

    print(Fore.YELLOW ,"Argument passed: ", sys.argv[1], "\n", Fore.RESET)

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)


date_inp = sys.argv[1]

date_format = '%Y-%m-%d'
#today_date = datetime.now().strftime(date_format)

now_pst = datetime.now(pytz.utc).astimezone(pytz.timezone('US/Pacific'))
today_date = now_pst.strftime(date_format)

downloads_folder = os.path.expanduser('~/Downloads')


from_date = datetime.strptime(date_inp, date_format)

from_date_query = from_date
till_date_query = from_date + timedelta(days=1)

from_date_query = from_date_query.strftime(date_format)
till_date_query = till_date_query.strftime(date_format)


print("------------------------------------------------------------")
delete_today_date_csv_files(date_format, today_date, downloads_folder)
print("------------------------------------------------------------")


#########################--- A1-Open_Still_bugs ---------------------------------------------------------------------

time.sleep(2)

#  Bugs Created on Date passed in the Input
webbrowser.open(f'http://bugz.mvista.com/buglist.cgi?keywords=Security;keywords_type=allwords;known_name=AutoScriptCheck;emailtype1=substring;chfieldfrom={from_date_query};chfieldto={till_date_query};chfield=%5BBug%20creation%5D;emailassigned_to1=1;email1=secbugz%40mvista.com;bug_status=NEW;bug_status=ASSIGNED;bug_status=IN_REVIEW;bug_status=SYNC_REQ;bug_status=REOPENED;component=Kernel;product=CGX%202.6;product=CGX%203.1;product=CGX%204.0;product=CGX%205.0')
time.sleep(1)

### RENAME FILE

old_file_name = f'bugs-{today_date}.csv'					# Construct the original and new file paths
old_file_path = os.path.join(downloads_folder, old_file_name)

for i in range(delay_time_pageload):						# takes time to load the data
    time.sleep(1)
    if os.path.exists(old_file_path):						# break from sleep, if the file is downloaded & exists
        time.sleep(1)
        break

new_file_name = 'csv_dnld_Created_bugs.csv'
new_file_path = os.path.join(downloads_folder, new_file_name)


if os.path.exists(old_file_path):						# Check if the old file exists
    try:
        os.rename(old_file_path, new_file_path)				        # Rename the file
        print(f"Renamed file: {old_file_path} to {new_file_path}")

    except Exception as e:
        print(Fore.RED, f"Error renaming file: {e}", Fore.RESET)
else:
    print(Fore.RED, f"\n\t\t\t\t File not found to rename: {old_file_path}", Fore.RESET)

print("\n\n")

