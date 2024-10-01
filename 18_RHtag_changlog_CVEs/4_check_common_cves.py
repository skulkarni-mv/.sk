#!/usr/bin/python3

import time


def extract_cve_ids_from_file(filename):
    cve_ids = set()  # Use a set to store CVE IDs for uniqueness

    with open(filename, 'r') as file:
        for line in file:
            # Strip whitespace and check for CVE IDs
            line = line.strip()
            if line.startswith("CVE-"):
                cve_ids.add(line)  # Add CVE ID to the set

    return cve_ids


print("\n Usage: ./4_check_common_cves.py \n")
print(" This code will compare CVE ids present in files 'file1_changelog_cves' & 'file2_bugz_extracted_cves' & Print them on screen\n\n")
time.sleep(2)


# Extract CVE IDs from both files
file1_cve_ids = extract_cve_ids_from_file('file1_changelog_cves')
file2_cve_ids = extract_cve_ids_from_file('file2_bugz_extracted_cves')

# Find common CVE IDs
common_cve_ids = file1_cve_ids.intersection(file2_cve_ids)

print("Common CVE IDs in both files:\n")
for cve in common_cve_ids:
    print(cve)


# Write the unique CVE IDs to a new file
with open('file_common_cves', 'w') as output_file:
    for cve in common_cve_ids:
        output_file.write(f"{cve}\n")

print("\n Common CVE IDs have been written to 'file_common_cves' \n")

