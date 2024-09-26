#!/usr/bin/python3

import re

# Read the file content
with open('RH_changelog_Input.txt', 'r') as file:
    data = file.read()

# Store results while maintaining combined order
results = []
cve_ids = set()

pattern = r'{(CVE-\d{4}-\d+)}'

# Find all relevant lines and CVE IDs in the data
for line in data.splitlines():
    if line.startswith('+*'):
        results.append("")  # Add a newline before the '+*' line
        results.append(line)  # Add lines starting with '+*'
    elif line.startswith('+-'):
        matches = re.findall(pattern, line)
        for match in matches:
            if match not in cve_ids:
                cve_ids.add(match)  # Add CVE ID to the set
                results.append(match)  # Add CVE ID to results to maintain order

# Print combined results
print("Combined results (lines and unique CVE IDs):")
for item in results:
    print(item)

