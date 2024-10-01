#!/usr/bin/python3

import re
import argparse
import time

def extract_cve_info(file_path):
    try:
        # Read the file content
        with open(file_path, 'r') as file:
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

        return results

    except FileNotFoundError:
        print(f"Error: The file '{file_path}' was not found.")
        return []
    except Exception as e:
        print(f"An error occurred: {e}")
        return []

if __name__ == "__main__":
    # Set up argument parsing

    print("\n Usage: ./1_CVEs_from_changelog.py <Changlog_file>\n")

    parser = argparse.ArgumentParser(description="Extract CVE IDs from a changelog file.")
    parser.add_argument('Changlog_file', type=str, help="Path to the changelog file")
    args = parser.parse_args()

    print(" This code will extract the CVE ids available in the changelog data & will be stored in file 'file1_changelog_cves'\n\n")
    time.sleep(2)

    combined_results = extract_cve_info(args.Changlog_file)

    # Print combined results
    print("Combined results (lines and unique CVE IDs):")
    for item in combined_results:
        print(item)

    # Write the combined results to a new file
    with open('file1_changelog_cves', 'w') as output_file:
        for item in combined_results:
            output_file.write(f"{item}\n")

    print("\nResults have been written to 'file1_changelog_cves'.")
