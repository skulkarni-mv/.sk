#!/bin/bash


RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
BLUE='\033[34m'
MAGENTA='\033[35m'
CYAN='\033[36m'
RESET='\033[0m'

current_time=$(date '+%H:%M')
echo "Start Time: "$current_time

execute_at_time="08:30"

# Main loop
while true; do
    # Get the current time in HH:MM format
    current_time=$(date '+%H:%M')

    # Check if it's 08:30 AM
    if [ "$current_time" == "$execute_at_time" ] || [ -f "manual_yday.file" ] || [ -f "manual_today.file" ] ; then

	if [ "$current_time" == "$execute_at_time" ]; then

		pass_date=$(date -d "yesterday" '+%Y-%m-%d')		# Get IST yesterday's date

	elif [ -f "manual_yday.file" ]; then
		rm "manual_yday.file"

		export TZ='America/Los_Angeles' 			# Note: This does not account for DST
		yest_date_pst=$(TZ='Etc/GMT+7' date -d "yesterday" '+%Y-%m-%d' )

		echo ""
		echo "Running Manually with Yesterday's PST Date: $yest_date_pst"
		pass_date=$yest_date_pst

		unset TZ						# Reset the timezone to the system's default
		echo "Resetting Timezone"
		echo ""

	elif [ -f "manual_today.file" ]; then
		rm "manual_today.file"

		export TZ='America/Los_Angeles' 			# Note: This does not account for DST
		today_date_pst=$(TZ='Etc/GMT+7' date '+%Y-%m-%d' )

		echo ""
		echo "Running Manually with Today's PST Date: $today_date_pst"
		pass_date=$today_date_pst

		unset TZ						# Reset the timezone to the system's default
		echo "Resetting Timezone"
		echo ""
	fi

        echo "Current Time: "$current_time

        # Call the Python script with yesterday's date as an argument
        echo ""
        echo -e "${GREEN} Running Python script with date: $pass_date ${RESET}"


        python3 weblinks.py "$pass_date"
        sleep 2

        if [ -f "/home/shubham/Downloads/csv_dnld_Created_bugs.csv" ]; then	# If csv file was downloaded & renamed by 'weblinks.py'
            cd ..								# Go to Parent dir to run './run1_stable_finder_bugz.out'

#            ./run1_stable_finder_bugz.out /home/shubham/Downloads/csv_dnld_Created_bugs.csv --maildata

            cd - > /dev/null							# Imp to come back to the original dir, > /dev/null as cmd prints
#            echo ""
#            echo "********"
            echo -e "${YELLOW}  Sent Mail ${RESET}"
	fi

        echo -e  "${GREEN}  Remember... this script is running in while() loop... ${RESET}"
        echo ""
        echo "***********************************************************************************************************************************"
        echo ""

        # Sleep for 60 seconds to avoid multiple executions within the same minute
        sleep 25	# 30+3 seconds sleep already done in 'weblinks.py' code / 2 secs in this file


    else		# take the sleep for minute function inside if-else. Otherwise, when if matches, total waittime becomes 2 mins not 1 min

        # Sleep for 1 minute before checking the time again
        sleep 60
    fi

done
