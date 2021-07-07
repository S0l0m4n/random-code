#!/bin/bash
# Use this function to upload a schedule for use by Econa
# TODO: terminate from get_profile function in case of bad env

BASE_SCH=schedule252.json
DB_TABLE=Schedule
DEV_PROFILE=
QA_PROFILE=OTARunnerQA

get_profile () {
    if [[ $1 == dev ]]
    then
        PROFILE=$DEV_PROFILE
    elif [[ $1 == qa ]]
    then
        PROFILE=$QA_PROFILE
    else
        "Bad environment: choose 'dev' or 'qa'"
        exit
    fi
}

if [ "$1" == "--help" ]
then
    echo "$0 --env [dev|qa] --pid <property-id>"
    exit
fi

while [[ $# > 0 ]]
do
    case $1 in
        "--env") # determine correct profile from environment
            shift
            get_profile $1
            ;;

        "--pid") # set property id
            shift
            PROPID=$1
            FILENAME=$PROPID-$BASE_SCH
            ;;

        *) # unknown option
            echo "Bad option: $1"
            exit
            ;;
    esac
    shift # get next argument
done

if [[ -z "$PROPID" ]]
then
    echo "Please supply the property ID"
    exit
fi

cp $BASE_SCH $FILENAME
echo "Upload schedule for property ID $PROPID"
sed -i "s/|PROPID|/$PROPID/" $FILENAME
sed -i "s/|GUID|/$(uuidgen)/" $FILENAME

aws dynamodb put-item --table-name $DB_TABLE \
    --item "file://$FILENAME" --profile="$PROFILE"
