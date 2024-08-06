#!/usr/bin/python3

from bs4 import BeautifulSoup
import sys
import requests
import argparse
from colorama import Fore
from colorama import Style

from selenium import webdriver
from time import sleep


def check_if_cve_rejected(cve):

    url = f"https://www.cve.org/CVERecord?id={cve}"

    # Initialize a headless browser
    options = webdriver.ChromeOptions()

    options.add_argument('headless')  # Run in headless mode, i.e., without a GUI
    sleep(1)

    driver = webdriver.Chrome(options=options)

    driver.get(url)
    sleep(1)

    driver.implicitly_wait(10)		# Adjust time
    sleep(1)

    html = driver.page_source
    sleep(1)

    soup = BeautifulSoup(html, 'html.parser')

    driver.quit()

#    print(soup)
#    return
#    link_text = soup.find_all('span', { 'class': 'has-text-weight-bold'})
#    link_text = soup.find_all('span', { 'class': 'has-text-weight-semibold'})

### CHATGPT

    spans = soup.find_all('span')

# Initialize variables to hold the results
#    assigner = None
    assignee = None
    published_date = None
    rejected_date = None
    updated_date = None
    description = None
    rejected_reason = None

    FinalComment = None

# Iterate through the span elements
    for i, span in enumerate(spans):
        text = span.get_text(strip=True)

    # Check if the text contains "Assigner: "
        if "Assigner:" in text:
#            assigner = text
        # Get the next span element for the assignee
            if i + 1 < len(spans):
                assignee = spans[i + 1].get_text(strip=True)
            break


    # Extract Published Date
    published_span = soup.find('span', string=lambda x: x and 'Published: ' in x)
    if published_span:
        published_time = published_span.find_next('time')
        if published_time:
            published_date = published_time.get_text(strip=True)

    # Extract Rejected Date
    rejected_span = soup.find('span', string=lambda x: x and 'Rejected: ' in x)
    if rejected_span:
        rejected_time = rejected_span.find_next('time')
        if rejected_time:
            rejected_date = rejected_time.get_text(strip=True)

    # Extract Updated Date
    updated_span = soup.find('span', string=lambda x: x and 'Updated: ' in x)
    if updated_span:
        updated_time = updated_span.find_next('time')
        if updated_time:
            updated_date = updated_time.get_text(strip=True)

    # Extract Description
    description_div = soup.find('div', id='cve-desciption')
    if description_div:
        description_title = description_div.find('h4', string='Description')
        if description_title:
            description_paragraph = description_div.find('p')
            if description_paragraph:
                description = description_paragraph.get_text(strip=True)


    # Extract Rejected Reason
    rejected_reason_span = soup.find('span', string=lambda x: x and 'Rejected Reason:' in x)
    if rejected_reason_span:
        # Find the next sibling span that contains the rejected reason text
        next_span = rejected_reason_span.find_next_sibling('span')
        if next_span:
            rejected_reason = next_span.get_text(strip=True)


# Print the extracted information
    """
#    print(f'Assigner: {assigner}')
    print(f'Assignee: {assignee}')
    print(f'Published: {published_date}')
    print(f'Updated: {updated_date}')
    print(f'Description: {description}')
    print(f'Rejected Reason: {rejected_reason}')
    """

    if assignee and (published_date or rejected_date or updated_date) and (description or rejected_reason):

        assignee = assignee[0].upper() + assignee[1:]

        FinalComment = 'Ref: '+'https://www.cve.org/CVERecord?id='+sys.argv[1] + '\n\n'

        FinalComment = FinalComment + 'Assigner: ' + f'{assignee}' + '  '

        if published_date:
            FinalComment = FinalComment + 'Published: ' + f'{published_date}' + '  '

        if rejected_date:
            FinalComment = FinalComment + 'Rejected: ' + f'{rejected_date}' + '  '

        if updated_date:
            FinalComment = FinalComment + 'Updated: ' + f'{updated_date}'

        FinalComment = FinalComment + '\n'

        if description:
            FinalComment = FinalComment + 'Description: ' + f'{description}'

        if rejected_reason:
            if description:
                FinalComment = FinalComment + '\n'

            FinalComment = FinalComment + 'Rejected Reason: ' + f'{rejected_reason}'

#        FinalComment = FinalComment + ' "'

        if not args.optional:
            print(FinalComment)
        if args.optional:
            fp=open('temp_comment_file.tmp', 'a+')
            fp.write('"\n')
            fp.write(FinalComment)
            fp.write('\n"')
            fp.close()


### Main
try:
    parser = argparse.ArgumentParser()

    parser.add_argument("CVE_num", type=str)
    parser.add_argument('--optional', type=str, help='Redirect Output to the file')

    args = parser.parse_args()

#    print("Argument passed: ", sys.argv[1], " -  ... "+ Fore.CYAN+ "https://ubuntu.com/security/"+sys.argv[1] +Fore.RESET)
    if not args.optional:
        print("\t\t\t\t\t\t\t "+ Fore.CYAN+ "https://www.cve.org/CVERecord?id="+sys.argv[1] +Fore.RESET)

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)

cve = sys.argv[1]
check_if_cve_rejected(cve)

