#!/bin/bash/python3

import json
import os
import requests

API_KEY = 'c2hmYVY5bkpDbXNrWVpWRlUyelZGeU9tRTJ1Y050cDN1NTZhcVJ5Ug=='
DONE_SYMBOL = '#'

headers = {
  'X-CSCAPI-KEY': API_KEY
}

def get_countries():
    url = 'https://api.countrystatecity.in/v1/countries'
    response = requests.request('GET', url, headers=headers)
    countries = []
    for item in json.loads(response.text):
        countries.append({'name': item['name'], 'iso2': item['iso2']})
    return countries

def get_country_states(code):
    url = \
        'https://api.countrystatecity.in/v1/countries/{}/states'.format(code)
    response = requests.request('GET', url, headers=headers)
    states = []
    for item in json.loads(response.text):
        states.append({'name': item['name'], 'iso2': item['iso2']})
    return states

def get_state_cities(country, state):
    url = 'https://api.countrystatecity.in/v1/countries/{}/states/{}/cities'.format(country, state)
    response = requests.request('GET', url, headers=headers)
    cities = []
    try:
        cities = [ item['name'] for item in json.loads(response.text) ]
    except TypeError:
        print("ERROR: {} / {}".format(country, state))
    return cities

def get_all_cities_for_country(country):
    cities = {}
    for state in get_country_states(country):
        name = state['name']
        cities[name] = get_state_cities(country, state['iso2'])
    return cities

# Arg: `countries` is a list of ISO2 country codes, e.g. ['US', 'AU', 'TW']
def generate_all_city_lists(countries):
    country_data = {}
    for cc in countries:  # cc == country_code
        print("Handling {}...".format(cc))
        country_data[f"{cc}"] = get_all_cities_for_country(cc)
    return country_data

# Open the given file and write on a new line the following:
#   - the country code
#   - the done symbol ('#') after a space
#   - newline character afterwards
# e.g. For the country Australia with code 'AU', we would write:
#       AU #
def mark_country_as_done(record_file, country):
    with open(record_file, 'a') as f:
        f.write("{} {}\n".format(country, DONE_SYMBOL))

# Open the given file and search for the country code with the done symbol
# e.g. AU #
def is_country_done(record_file, country):
    if not os.path.isfile(record_file):
        # file does not exist
        return False

    with open(record_file, 'r') as f:
        for line in f:
            if line[:2] != country:
                # check next line
                continue
            # check done symbol
            if len(line) == 5 and line[3] == DONE_SYMBOL:
                # code found, stop now
                return True
            else:
                raise Exception(
                    "ERROR: Garbled line for {} in record file {}"
                    .format(country, record_file))

    # code not found in file
    return False

COUNTRY_RECORD_FILE = "country_records.txt"
COUNTRY_DATA_DIR = "data"

if __name__ == '__main__':
    assert os.path.isdir(COUNTRY_DATA_DIR), \
            "ERROR: Directory `{}` does not exist!".format(COUNTRY_DATA_DIR)
    countries = get_countries()

    codes = ['MU', 'UA']
#   codes = countries.keys()

    # write each country's cities to a unique file
    print("Generating city list for each country...")
    for code in codes:
        # check if the country has already been handled
        if is_country_done(COUNTRY_RECORD_FILE, code):
            # skip to the next code
            continue

        # get the country name, to append to each line
        country = \
            [ item['name'] for item in countries if item['iso2'] == code ].pop()

        # get all the cities first, grouped by state
        print("Handling {}...".format(country))
        cities = get_all_cities_for_country(code)

        # write cities in the format:
        #   city, state, country
        print("Writing out cities for {}...".format(country))
        with open("{}/{}.txt".format(COUNTRY_DATA_DIR, code), 'w') as f:
            # write the country alone, first
            f.write(country + "\n")
            for state in cities.keys():
                # next, write the state
                f.write(state + ", " + country + "\n" )
                if len(cities[state]) < 2:
                    # no city, so don't write anything more
                    pass
                else:
                    for city in cities[state]:
                        # finally, write each city and state (and country)
                        f.write(city + ", " + state + ", " + country + "\n")

        # mark the country as done for future running
        mark_country_as_done(COUNTRY_RECORD_FILE, code)
