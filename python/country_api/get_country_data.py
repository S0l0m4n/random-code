#!/bin/bash/python3

import json
import requests

API_KEY = 'c2hmYVY5bkpDbXNrWVpWRlUyelZGeU9tRTJ1Y050cDN1NTZhcVJ5Ug=='

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

if __name__ == '__main__':
    pass
