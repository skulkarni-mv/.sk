#!/bin/bash

# Function to get yesterday's date in YYYY-MM-DD format
get_yesterday_date() {
    date -d "yesterday" '+%Y-%m-%d'
}

# Main loop
while true; do
    # Get the current time in HH:MM format
    current_time=$(date '+%H:%M')

    # Check if it's 12:30 PM
    if [ "$current_time" == "12:30" ]; then
        # Get yesterday's date
        yesterday_date=$(get_yesterday_date)
        echo $current_time

        # Call the Python script with yesterday's date as an argument
        echo ""
        echo "Running Python script with date: $yesterday_date"

        python3 weblinks.py "$yesterday_date" "$yesterday_date"

        echo ""
        echo "--------- following csv files are downloaded using the script ----------"
        echo ""
        ls -lh /home/shubham/Downloads/csv_dnld*
        echo ""
        echo "------ 'weblinks.py' script execution completed, Sending mail now ------"
        echo "Subject: Script Execution Completed for Today" > output ; echo "" >> output ; ls -lh /home/shubham/Downloads/csv_dnld* >> output
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

