#!/usr/bin/python3

import csv
import re
import argparse
from collections import OrderedDict
import time

# Function to extract unique CVE IDs
def extract_unique_cve_ids(filename):
    cve_ids = OrderedDict()  # Using OrderedDict to maintain order

    with open(filename, 'r') as file:
        reader = csv.reader(file)

        for row in reader:
            for entry in row:  # Check each entry in the row
                # Extract CVE IDs using regex
                matches = re.findall(r'(CVE-\d{4}-\d+)', entry)
                for match in matches:
                    if match not in cve_ids:
                        cve_ids[match] = None  # Store in OrderedDict to maintain order

    return list(cve_ids.keys())  # Return only the unique CVE IDs


if __name__ == "__main__":

    print("\n Usage: ./3_CVEs_from_CSV_bugz.py <CSV_file_path>\n")

    parser = argparse.ArgumentParser(description="Extract unique CVE IDs from a CSV file.")
    parser.add_argument('filename', type=str, help="Path to the CSV file containing CVE IDs")
    args = parser.parse_args()

    print(" This code will extract the CVE ids present in the CSV file, Print them on screen & write to file 'file2_bugz_extracted_cves'\n\n")
    time.sleep(2)

    # Read CVE IDs from the provided filename
    unique_cve_ids = extract_unique_cve_ids(args.filename)

    #print("Unique CVE IDs:")
    for cve in unique_cve_ids:
        print(cve)

    # Write the unique CVE IDs to a new file
    with open('file2_bugz_extracted_cves', 'w') as output_file:
        for cve in unique_cve_ids:
            output_file.write(f"{cve}\n")

    print("\n Unique CVE IDs have been written to 'file2_bugz_extracted_cves' \n")

