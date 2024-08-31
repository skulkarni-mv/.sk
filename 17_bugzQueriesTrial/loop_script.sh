#!/bin/bash


GREEN='\033[32m'
RESET='\033[0m'


# Main loop
while true; do
    # Get the current time in HH:MM format
    current_time=$(date '+%H:%M')

    # Check if it's 12:30 PM
    if [ "$current_time" == "12:30" ] || [ -f "manual_yday.file" ] || [ -f "manual_today.file" ] ; then

	if [ "$current_time" == "12:30" ]; then

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

        echo $current_time

        # Call the Python script with yesterday's date as an argument
        echo ""
        echo -e "${GREEN} Running Python script with date: $pass_date ${RESET}"

        python3 weblinks.py "$pass_date" "$pass_date"

        echo ""
        echo "--------- following csv files are downloaded using the script ----------"
        echo ""
        ls -lh ~/Downloads/csv_dnld* | grep $pass_date
        echo ""
        echo "------ 'weblinks.py' script execution completed, Sending mail now ------"
        echo "Subject: Script Execution Completed for Today" > output ; echo "" >> output
        ls -lh ~/Downloads/csv_dnld* | grep $pass_date >> output ; echo "" >> output; echo "" >> output; echo "" >> output
        ls -lh ~/Downloads/csv_dnld* >> output
        echo ""
        sleep 5 &&  git send-email --to=skulkarni@mvista.com --confirm=never output > /dev/null ; rm output
        echo ""
        echo "_________________________________________________________________________________________________________________________"
        echo ""

        # Sleep for 60 seconds to avoid multiple executions within the same minute
        sleep 60
    fi

    # Sleep for 1 minute before checking the time again
    sleep 60
done

