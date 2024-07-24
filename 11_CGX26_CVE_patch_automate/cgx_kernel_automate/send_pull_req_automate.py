#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import subprocess
import getpass
import requests
import pexpect
import re
import os

bug_no = ''
revision = ''
p_count = ''
start = ''
start_c = ''
upstream_br = ''
reviewer_ids = ''

contrib = ''
msg = ''
contrib_url = ''
tag = ''
username = ''
mvista_id = ''
debug = ''
tag_is_created = ''

repo_details = [
    {
        'repo': 'CGE7',
        'origin': 'gitcge7.mvista.com:/mvista/git/cge7/kernel/mvl7kernel.git',
        'pushto': 'gitcge7.mvista.com:/mvista/git/cge7/contrib/kernel.git',
        'url': 'git://gitcge7.mvista.com/cge7/contrib/kernel.git',
        'web_url_tag': 'http://gitcge7.mvista.com/cgit/cge7/contrib/kernel.git/refs/tags',
        },
    {
        'repo': 'CGX1.8',
        'origin': 'gitcgx.mvista.com:/mvista/git/cgx/CGX/kernels/linux-mvista-1.8.git',
        'pushto': 'gitcgx.mvista.com:/mvista/git/cgx/contrib/kernel.git',
        'url': 'git://gitcgx.mvista.com/cgx/contrib/kernel.git',
        'web_url_tag': 'http://gitcgx.mvista.com/cgit/contrib/kernel.git/refs/tags',
        },
    {
        'repo': 'CGX2.0',
        'origin': 'gitcgx.mvista.com:/mvista/git/cgx/CGX2.0/kernel/linux-mvista-2.0.git',
        'pushto': 'gitcgx.mvista.com:/mvista/git/cgx/contrib/kernel.git',
        'url': 'git://gitcgx.mvista.com/cgx/contrib/kernel.git',
        'web_url_tag': 'http://gitcgx.mvista.com/cgit/contrib/kernel.git/refs/tags',
        },
    {
        'repo': 'CGX2.2',
        'origin': 'gitcgx.mvista.com:/mvista/git/cgx/CGX2.2/kernel/linux-mvista-2.2.git',
        'pushto': 'gitcgx.mvista.com:/mvista/git/cgx/contrib/kernel.git',
        'url': 'git://gitcgx.mvista.com/cgx/contrib/kernel.git',
        'web_url_tag': 'http://gitcgx.mvista.com/cgit/contrib/kernel.git/refs/tags',
        },
    {
        'repo': 'CGX2.4',
        'origin': 'gitcgx.mvista.com:/mvista/git/cgx/CGX2.4/kernel/linux-mvista-2.4.git',
        'pushto': 'gitcgx.mvista.com:/mvista/git/cgx/contrib/kernel.git',
        'url': 'git://gitcgx.mvista.com/cgx/contrib/kernel.git',
        'web_url_tag': 'http://gitcgx.mvista.com/cgit/contrib/kernel.git/refs/tags',
        },
    {
        'repo': 'CGX2.6',
        'origin': 'gitcgx.mvista.com:/mvista/git/cgx/CGX2.6/kernel/linux-mvista-2.6.git',
        'pushto': 'gitcgx.mvista.com:/mvista/git/cgx/contrib/kernel.git',
        'url': 'git://gitcgx.mvista.com/cgx/contrib/kernel.git',
        'web_url_tag': 'http://gitcgx.mvista.com/cgit/contrib/kernel.git/refs/tags',
        },
    {
        'repo': 'CGX3.1',
        'origin': 'gitcgx.mvista.com:/mvista/git/cgx/CGX3.1/kernel/linux-mvista-3.1.git',
        'pushto': 'gitcgx.mvista.com:/mvista/git/cgx/contrib/kernel.git',
        'url': 'git://gitcgx.mvista.com/cgx/contrib/kernel.git',
        'web_url_tag': 'http://gitcgx.mvista.com/cgit/contrib/kernel.git/refs/tags',
        },
    {
        'repo': 'CGX4.0',
        'origin': 'gitcgx.mvista.com:/mvista/git/cgx/CGX4.0/kernel/linux-mvista-4.0.git',
        'pushto': 'gitcgx.mvista.com:/mvista/git/cgx/contrib/kernel.git',
        'url': 'git://gitcgx.mvista.com/cgx/contrib/kernel.git',
        'web_url_tag': 'http://gitcgx.mvista.com/cgit/contrib/kernel.git/refs/tags',
        },
]


def dbg_print(str):
    if debug == 1:
        if str:
            print(str)


def error_exit(prstr):
    print(prstr)
    print("Exiting..")
    if tag_is_created == 1:
        run_cmd('git tag -d %s > /dev/null' % (tag))
    sys.exit(1)


def run_cmd(cmd, *argv):
    leng = len(argv)
    sstr = ""
    fstr = ""

    if leng:
        fstr = argv[0]
        if leng > 1:
            sstr = argv[1]

    try:
        op = subprocess.check_output(cmd, shell=True, universal_newlines=True)
        dbg_print(sstr)
        return op
    except subprocess.CalledProcessError as e:
        error_exit(fstr)


def find(lst, key, value):
    for i, dic in enumerate(lst):
        if dic[key] == value:
            return i
    return -1


def auto_find_rev_from_contrib(tag_url, remote_branch):
    tag_fmt_str = '%s-%s-V' % (remote_branch, bug_no)
    if revision == 'None':
        with requests.session() as s:
            try:
                r = s.get(tag_url, timeout=30)
                soup = bs(r.text, 'lxml')
                tag_list = []

                for line in soup.find_all('a'):
                    tag_str = line.get_text()
                    pattern = re.compile(tag_fmt_str + '[1-9][0-9]*$')
                    if re.search(pattern, tag_str):
                        tag_list.append(tag_str.replace(tag_fmt_str, ""))

                tag_list = list(map(int, tag_list))

            except requests.exceptions.Timeout:
                error_exit("Connection timed out")
            except requests.exceptions.RequestException as e:
                error_exit(e)
    else:
        return revision

    if tag_list:
        latest_rev = max(tag_list)
        new_rev = latest_rev + 1
    else:
        new_rev = 1

    dbg_print("Auto found revision = %d from %s" % (new_rev, tag_url))
    return new_rev


def form_repo_data(idx, remote_branch):
    global contrib, msg, contrib_url, tag
    dbg_print("%s repo identified." % (repo_details[idx]['repo']))
    dbg_print("Your changes target the remote branch:%s" % (remote_branch))
    contrib = repo_details[idx]['pushto']
    contrib_url = repo_details[idx]['url']
    rev = auto_find_rev_from_contrib(repo_details[idx]['web_url_tag'], remote_branch)
    msg = 'Merge to %s bug %s' % (remote_branch, bug_no)
    tag = '%s-%s-V%s' % (remote_branch, bug_no, rev)

def identify_user():
    global username, mvista_id
    username = run_cmd('git config user.name', "Please configure username for the git repo").splitlines()[0]
    mvista_id = run_cmd('git config user.email', "Please configure email-id for the git repo").splitlines()[0]
    if not (re.match(r'.*.+@mvista.com', mvista_id)):
        error_exit("User's email-ID is not from the domain mvista.com")

    mvista_id = mvista_id.split('@')[0]


def identify_repo():
    global upstream_br
    identify_user()
    url = run_cmd('git config --get remote.origin.url', "Please run from a valid git repository").splitlines()[0]
    match = re.compile('.*'+mvista_id+'@.*.')
    if not (re.match(match, url)):
            error_exit("Not an MV type repo\n")

    idx = find(repo_details, 'origin', url.split('@')[1])
    if idx == -1:
        error_exit("Unable to find the repo type\n")

    # Now identify the remote tracking branch of the current branch
    upstream_br = run_cmd('git rev-parse --abbrev-ref --symbolic-full-name @{u}', "Unable to find the remote branch").splitlines()[0]
    form_repo_data(idx, upstream_br.split('/', 1)[1])


def auto_count_commits_to_push():
    global p_count
    p_count = run_cmd('git rev-list --count %s..' % upstream_br).splitlines()[0]
    if p_count == '0':
        error_exit("Nothing to push. Current branch is up to date with %s" % upstream_br)
    dbg_print('Found %s commits to push to contrib' % p_count)


def mark_start_commit():
    global start_c
    if p_count == 'None' and start == 'None':
        auto_count_commits_to_push()

    if p_count != 'None':
        start_c = 'HEAD~'+p_count
    elif start != 'None':
        start_c = start


def parse_args():
    global bug_no, revision, p_count, start, debug, reviewer_ids
    parser = argparse.ArgumentParser(description="Perform merge request on CGE7 or CGX repo",
            formatter_class=argparse.RawDescriptionHelpFormatter,
            epilog = """
To send the pull request to the gate-keeper
    $%(prog)s -b bug_number
    The options -n, -s, -r can be used to further refine your pull request.

To send the pull request for review(copying cge-dev)
    $%(prog)s -b bug_number --rr mvistaid_of_reviwer,cge-dev
    """)
    parser.add_argument('-b', help='Bug number', required=True, type=int)
    parser.add_argument('-r', help='Patch revision', required=False, type=int)
    parser.add_argument('-n', help='No of patches you are pushing', required=False, type=int)
    parser.add_argument('-s', help='Start commit for merge request', required=False, type=str)
    parser.add_argument('--rr', help='Send the merge request for review', required=False, type=str)
    parser.add_argument('-v', help='Print verbose messages', action="store_true")
    args = vars(parser.parse_args())

    if args['v']:
        debug = 1

    bug_no = str(args['b'])
    revision = str(args['r'])
    p_count = str(args['n'])
    start = str(args['s'])
    reviewer_ids = str(args['rr'])


def check_err(err, got):
    if err == got:
        error_exit(err)


def update_bugz_merge_req_fields(uname, pword, cmnt):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    upd_d = {
               'id': bug_no,
'status_whiteboard': 'GitMergeRequest',
       'bug_status': 'SYNC_REQ',
            'newcc': 'cge7-kernel-gatekeepers@mvista.com, cminyard',
    }

    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            check_err("Invalid Username Or Password", soup.title.string)

            r = s.get(bugz_login_url)
            soup = bs(r.text, 'lxml')
            upd_d['delta_ts'] = soup.find('input', {'name': 'delta_ts'}).get('value')
            upd_d['token'] = soup.find('input', {'name': 'token'}).get('value')
            upd_d['comment'] = cmnt

            r = s.post(bugz_post_url, data=upd_d)
        except requests.exceptions.Timeout:
            error_exit("Connection timed out")
        except requests.exceptions.RequestException as e:
            error_exit(e)


## -> Added by Shubham

def check_bugz_current_status(uname, pword, bug_no):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            check_err("Invalid Username Or Password", soup.title.string)

            result = soup.find('div', {'id': 'status'}).find('select', {'name': 'bug_status'})
            if result:
                result_str = ' '.join(result.get_text().split())
                result_cur = result_str.split()[0]
                result_fut = result_str.split(' ', 1)[1]
                print("\n\t Current status of the bug:", result_cur)
#                print(" Following changes in status are possible:", result_fut)

                if(result_cur == 'ASSIGNED'):
                    return 0

                else:
                    if 'ASSIGNED' in result_fut:
                        print('\t\t Bugz status is not Assigned, Updating Bugz status first as per the process...')
                        return 1
                    else:
                        error_exit(' Bugz status is: '+ result_cur+ '. As per the process, status should be "ASSIGNED".')

        except requests.exceptions.Timeout:
            error_exit("Connection timed out")
        except requests.exceptions.RequestException as e:
            error_exit(e)


def update_bugz_status_assigned(uname, pword, bug_no):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    upd_d = {
        'id': bug_no,
'bug_status': 'ASSIGNED',
    }

    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            check_err("Invalid Username Or Password", soup.title.string)

            r = s.get(bugz_login_url)
            soup = bs(r.text, 'lxml')
            upd_d['delta_ts'] = soup.find('input', {'name': 'delta_ts'}).get('value')
            upd_d['token'] = soup.find('input', {'name': 'token'}).get('value')
            upd_d['comment'] = 'changing status to ASSIGNED'

            r = s.post(bugz_post_url, data=upd_d)
        except requests.exceptions.Timeout:
            error_exit("Connection timed out")
        except requests.exceptions.RequestException as e:
            error_exit(e)

## <- Added by Shubham


### MAIN

parse_args()
identify_repo()
mark_start_commit()

dbg_print("Generating pull request with the details")
dbg_print("----------------------------------------")
dbg_print("Username: %s" % (username))
dbg_print("Bugzilla-ID: %s" % (mvista_id))
dbg_print("----------------------------------------")

#bugz_pword = getpass.getpass('Please enter bugzilla password for %s:' % (mvista_id))	# Changed by Shubham
fp = open('cgx_kernel_automate/.bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()

## -> Added by Shubham

curr_bug_state = check_bugz_current_status(mvista_id, bugz_pword, bug_no)		# if 0 => Assigned, 1 => need to change

if (curr_bug_state == 1):
    update_bugz_status_assigned(mvista_id, bugz_pword, bug_no)

    curr_bug_state = check_bugz_current_status(mvista_id, bugz_pword, bug_no)
    if (curr_bug_state != 0):
        error_exit('Unable to proceed. Exiting..')

elif (curr_bug_state == 0):
    curr_bug_state = 0
else:
    error_exit('Unable to proceed. Exiting...')


print("----------------------------------------")
print("")
## <- Added by Shubham

run_cmd('git tag -m "%s" "%s"' % (msg, tag), 'Please run after manually deleting the tag', 'tag: %s created\n' % (tag))
tag_is_created = 1

dbg_print("Pushing the tag: %s to %s" % (tag, contrib_url))
cmd = 'git push "%s"@"%s" "+%s"' % (mvista_id, contrib, tag)
exp_str1 = 'password'
exp_str2 = 'fatal: Could not read from remote repository.'
exp_str3 = 'Permission denied, please try again.'
child = pexpect.spawn(cmd)
index = child.expect([exp_str1, exp_str2, pexpect.EOF], timeout=180)
if index == 0:
    child.sendline(bugz_pword)
else:
    child.close()
    error_exit("Unable to read from the remote git repo")

index = child.expect([pexpect.TIMEOUT, exp_str3, exp_str2, pexpect.EOF], timeout=1800)	# Changed from 180 to 1800 (30 mins)
if index == 3:
    dbg_print('tag pushed to %s\n' % (contrib))
elif index == 0:
    print("Timeout during pushing the tag to contrib, but continuing")
else:
    child.close()
    error_exit("Wrong password, Please try again..")

bugz_comment = run_cmd('git request-pull %s %s %s' % (start_c, contrib_url, tag))
dbg_print(bugz_comment)

dbg_print("Trying to update bugzilla fields for bug %s" % bug_no)

if reviewer_ids != 'None':
#    bugz_add_review_req_attachment(mvista_id, bugz_pword, bugz_comment)
#    print("Successfully posted the review request on bugzilla. Done.")
    print("The Review Request process is No Longer Supported as it's not correct review process. Exiting...")
    sys.exit(1)

else:
    update_bugz_merge_req_fields(mvista_id, bugz_pword, bugz_comment)
    print("Successfully posted the merge request on bugzilla. Done.")
									# -> Added by Shubham to store tag into file
    fp = open("cgx_kernel_automate/generated_details.txt", "w")
    fp.write(tag+'\n')
    fp.close()
									# <- Added by Shubham to store tag into file
